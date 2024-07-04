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

#ifndef RC_BEARER_TOKEN_H
#define RC_BEARER_TOKEN_H

#include <stdint.h>
#include <stdlib.h>
#include <curl/curl.h>

#define TOKEN_MAX_SIZE 2048

typedef enum {

    RC_TOKEN_OK,
    RC_TOKEN_UNINITIALIZED,
    RC_TOKEN_OUT_OF_SPACE,
    RC_MISSING_CREDENTIALS,
    RC_TOKEN_PARSING_ERROR,

    RC_CURL_INIT_FAILED,
    RC_CURL_TRANSFER_FAILED

} TokenError;

/**
 * Not using opaque typedef here, specifically so that
 * RC_TOKEN_SKELETON can initialize the struct inline
 * without function call / copy on return
 */

/**
 * Container struct that facilitates JWT flow
 * The only public interface is the RC_TOKEN_SKELETON macro
 * 
 * -- Declaration & Initialization --
 * RIGHT: BearerToken* token = RC_TOKEN_SKELETON();
 * WRONG: BearerToken* token; // this will cause a crash later.
 * 
 * Do not assume/directly modify its member variables or buffer
 * Do not malloc or free
 */
typedef struct {

    const char* server_url;

    char buffer[TOKEN_MAX_SIZE];
    const char* client_id;
    const char* client_secret;
    const char* jwt;

    const char* access_token;
    const char* token_type;
    time_t expires_in;
    size_t avail_size;

    TokenError s_token;
    char error[CURL_ERROR_SIZE];

} BearerToken;

#define RC_TOKEN_SKELETON() &(BearerToken) \
{                                          \
    .buffer = {0},                         \
    .server_url = RC_OAUTH_TOKEN,          \
    .client_id = RC_CLIENT_ID,             \
    .client_secret = RC_CLIENT_SECRET,     \
    .jwt = RC_JWT,                         \
    .expires_in = 0,                       \
    .avail_size = TOKEN_MAX_SIZE,          \
    .s_token = RC_TOKEN_UNINITIALIZED      \
}

#ifndef RINGEXTRACT_H // internal functions used within RingEXtract

/// @brief standard fetch token/set token routine
/// @param token pointer to a BearerToken struct (can be just a skeleton)
/// @param curl a CURL handle
/// @return TokenError code
TokenError rc_curl_set_token(BearerToken* token, CURL* curl);

/// @brief standard routine used by most of RingEXtract's data fetching API
/// @param token pointer to a BearerToken struct (can be just a skeleton)
/// @param curl a CURL handle
/// @return TokenError code
TokenError rc_curl_auto_perform(BearerToken* token, CURL* curl);

#endif // RINGEXTRACT_H

#endif // RC_BEARER_TOKEN_H
