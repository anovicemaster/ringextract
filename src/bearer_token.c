/**
 *  RingEXtract - RingEX C Interface for Data Extraction
 *  Copyright (C) 2024 Ian Wang
 *  
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <string.h>
#include <stdbool.h>

#include "bearer_token.h"
#include "rate_limiter.h"

#define MAX(X, Y) (X > Y ? X : Y)
#define MIN(X, Y) (X < Y ? X : Y)

/** Official response structure (application/json)
 * 
 *  {
 *      "access_token" : "U1BCMDFUMDRKV1MwMXxzLFSvXdw5PHMsVLEn_MrtcyxUsw",
 *      "token_type" : "bearer",
 *      "expires_in" : 7199,
 *      "refresh_token" : "U1BCMDFUMDRKV1MwMXxzLFL4ec6A0XMsUv9wLriecyxS_w",
 *      "refresh_token_expires_in" : 604799,
 *      "scope" : "AccountInfo CallLog ExtensionInfo Messages SMS",
 *      "owner_id" : "256440016"
 *  }
 * 
 */

// basically implementing a custom strtok here
static const char* rc_char_extract(char** json, size_t* n) {
    
    char* const origin = *json;
    const size_t range = *n;
    if (origin == NULL || range == 0) { return NULL; }

    char* cursor = memchr(*json, ':', range);
    if (cursor) { cursor++; }
    else { *json = NULL; return NULL; }

    const char* token = NULL;

    while (range > cursor - origin) {
        if (cursor[0] < '0') { cursor++; }
        else { token = cursor; break; }
    }

    while (range > cursor - origin) {
        if (cursor[0] >= '0') { cursor++; }
        else { cursor[0] = '\0'; break; }
    }

    *json = cursor;
    *n = range - (cursor - origin);
    return token;

}

static time_t rc_time_extract(char** json, size_t* n) {

    const char* token = rc_char_extract(json, n);

    if (token == NULL) { return 0; }
    else { return strtol(token, NULL, 10); }

}

static size_t rc_token_save(char* contents, size_t size, size_t nitems, void* userdata) {

    BearerToken* token = (BearerToken*)userdata;

    const size_t chunk_size = size * nitems;
    const size_t avail_size = token->avail_size;
    const size_t bytes_copy = MIN(chunk_size, avail_size);

    char* cursor = (char*)(token->client_id - avail_size);
    memcpy(cursor, contents, bytes_copy);
    token->avail_size -= bytes_copy;
    return chunk_size;

}

static inline TokenError rc_token_validate(BearerToken* token) {

    const time_t now = time(NULL);

    const bool access_token = token->access_token != NULL;
    const bool token_type   = token->token_type   != NULL;
    const bool expires_in   = token->expires_in    > now;

    if (access_token && token_type && expires_in) { return token->s_token; }
    else { token->s_token = RC_TOKEN_PARSING_ERROR; return token->s_token; }

}

static TokenError rc_token_request(BearerToken* token) {

    CURL* curl = curl_easy_init();

    if (curl) {

        curl_easy_setopt(curl, CURLOPT_URL       , token->server_url);
        curl_easy_setopt(curl, CURLOPT_USERNAME  , token->client_id);
        curl_easy_setopt(curl, CURLOPT_PASSWORD  , token->client_secret);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, token->jwt);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, rc_token_save);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, token);
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, token->error);
        curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1);

        rc_curl_set_limit(token, curl, 0, 0);
        curl_easy_cleanup(curl);

    } else { token->s_token = RC_CURL_INIT_FAILED; }

    if (token->s_token == RC_TOKEN_OK) {

        const size_t avail_size = token->client_id - token->buffer;
        size_t bytes = avail_size - token->avail_size;
        char* rest = token->buffer;

        token->access_token = rc_char_extract(&rest, &bytes);
        token->token_type   = rc_char_extract(&rest, &bytes);
        token->expires_in  += rc_time_extract(&rest, &bytes);

        token->avail_size = avail_size;
        return rc_token_validate(token);

    } else { return token->s_token; }

}

static const char* rc_token_memcpy(BearerToken* token, const char* s, const size_t n) {

    if (token->avail_size < n) {

        token->avail_size = 0;
        token->s_token = RC_TOKEN_OUT_OF_SPACE;
        return NULL;

    }

    token->avail_size -= n;
    return memcpy(token->buffer + token->avail_size, s, n);

}

static TokenError rc_token_materialize(BearerToken* token) {

#define POST_TEXT "grant_type=urn%3Aietf%3Aparams%3Aoauth%3Agrant-type%3Ajwt-bearer&assertion="
#define POST_SIZE sizeof(POST_TEXT) - 1

    if (token->s_token != RC_TOKEN_UNINITIALIZED) { return token->s_token; }
    else { token->s_token = RC_TOKEN_OK; }

    const char* id = token->client_id;
    const char* secret = token->client_secret;
    const char* jwt = token->jwt;

    if (id && secret && jwt) {

        token->jwt           = rc_token_memcpy(token, jwt, strlen(jwt) + 1);
        token->jwt           = rc_token_memcpy(token, POST_TEXT, POST_SIZE);
        token->client_secret = rc_token_memcpy(token, secret, strlen(secret) + 1);
        token->client_id     = rc_token_memcpy(token, id, strlen(id) + 1);

    } else { token->s_token = RC_MISSING_CREDENTIALS; }

    return token->s_token;

#undef POST_TEXT
#undef POST_SIZE

}

static TokenError rc_curl_set_error(BearerToken* token) {

    const char* message = NULL;

    switch (token->s_token) {

    case RC_TOKEN_OK:
        memset(token->error, 0, CURL_ERROR_SIZE);
        return token->s_token;

    case RC_TOKEN_UNINITIALIZED:
        message = "Token has not been materialized.";
        break;

    case RC_TOKEN_OUT_OF_SPACE:
        message = "Insufficient space: credentials are too long.";
        break;

    case RC_MISSING_CREDENTIALS:
        message = "Missing credentials.";
        break;

    case RC_TOKEN_PARSING_ERROR:
        message = "Token parsing error: unknown access token format.";
        break;

    case RC_CURL_INIT_FAILED:
        message = "CURL initialization failed.";
        break;

    case RC_CURL_TRANSFER_FAILED:
        // Error message is already written in buffer by libcurl
        // Fall through and return
    
    default:
        return token->s_token;

    }

    const size_t length = strlen(message) + 1;
    memcpy(token->error, message, length);
    return token->s_token;

}

TokenError rc_curl_set_token(BearerToken* token, CURL* curl) {

    if (rc_token_materialize(token) != RC_TOKEN_OK) { return token->s_token; }

    const time_t now = time(NULL);

    if (token->expires_in < now) {

        token->expires_in = now;
        rc_token_request(token);

    }

    if (token->s_token == RC_TOKEN_OK) {

        curl_easy_setopt(curl, CURLOPT_XOAUTH2_BEARER, token->access_token);
        curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BEARER);
        curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, token->error);
        curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1);

    }

    return token->s_token;

}

TokenError rc_curl_auto_perform(BearerToken* token, CURL* curl) {

    if (rc_curl_set_token(token, curl) == RC_TOKEN_OK)
    {   rc_curl_set_limit(token, curl, MAX_RETRY_ATTEMPT, MIN_RETRY_TIMEOUT); }
    
    return rc_curl_set_error(token);

}
