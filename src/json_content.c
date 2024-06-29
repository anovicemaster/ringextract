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

#include "json_content.h"

#define MUL(X, Y) (((X) / (Y) + 1) * (Y))

static size_t rc_curl_write_json(char* contents, size_t size, size_t nitems, void* userdata) {

    JsonContent* json = (JsonContent*)userdata;
    const size_t chunk_size = size * nitems;
    const size_t old_size = json->n_bytes;

    if (!chunk_size) { return 0; }
    const char* head = NULL;
    size_t n_bytes = 0;

    if (json->n_pages == 0 || json->n_chunk > 0) {

        head = contents;
        n_bytes = chunk_size;
        
    } else {
        
        head = memchr(contents, '[', chunk_size) + 1;
        if (head - 1) { n_bytes = chunk_size - (head - contents); }
        else { return chunk_size; }
        
    }

    if (n_bytes) { json->n_bytes += n_bytes; }
    else { json->n_chunk++; return chunk_size; }

    if (json->n_bytes >= json->total_size) {

        json->total_size = MUL(json->n_bytes, json->init_size);
        char* buffer = realloc(json->buffer, json->total_size);

        if (buffer) { json->buffer = buffer; }
        else { return 0; }

    }

    memcpy(json->buffer + old_size, head, n_bytes);
    json->n_chunk++;
    return chunk_size;

}

// in case last page is empty due to timing differences
static inline void rc_json_bridge(char* cursor, const char* buffer) {

    while (cursor-- > buffer) {

        if (*cursor == ',') { *cursor = ' '; }
        else if (*cursor == ' ') { continue; }
        else { break; }

    }

}

static inline void rc_json_reset(JsonContent* json) {

    json->n_bytes = 0;
    json->n_pages = 0;
    json->n_chunk = 0;
    json->url_next_page = NULL;

}

static void rc_curl_next_page(JsonContent* json) {

    if (json->n_bytes) {

        json->n_pages++;
        json->n_chunk = 0;

    } else { json->url_next_page = NULL; return; }

    char* cursor = json->buffer + json->n_bytes;
    cursor[0] = '\0';

    // a one-liner implementation of memrchr, for portability
    while (cursor-- > json->buffer && *cursor != ']');

    if (cursor > json->buffer) { // possibly paginated endpoints

        json->url_next_page = strstr(cursor + 1, "\"nextPage\"");
        if (!json->url_next_page) { return rc_json_bridge(cursor, json->buffer); }
        else { json->url_next_page += 10; }
        
        json->url_next_page = strstr(json->url_next_page, "https://");
        if (!json->url_next_page) { return rc_json_bridge(cursor, json->buffer); }

        size_t n = json->n_bytes - (json->url_next_page - json->buffer);
        json->n_bytes = cursor - json->buffer + 1;

        char* c = memchr(json->url_next_page, '\"', n);
        if (c) { c[0] = '\0'; cursor[0] = ','; }

    } else { json->url_next_page = NULL; } // non-paginated endpoints

}

void rc_json_get_buffer(BearerToken* token, JsonContent* json, const char* url) {

    CURL* curl = curl_easy_init();

    if (curl) {
        
        rc_json_reset(json);

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, rc_curl_write_json);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, json);
        
    } else { token->s_token = RC_CURL_INIT_FAILED; return; }

    do {

        if (rc_curl_auto_perform(token, curl) != RC_TOKEN_OK) { break; }
        else { rc_curl_next_page(json); }

        curl_easy_setopt(curl, CURLOPT_URL, json->url_next_page);

    } while (json->url_next_page);
    
    return curl_easy_cleanup(curl);
    
}

const char* rc_json_get_file(BearerToken* token, const char* file, const char* url) {

    CURL* curl = curl_easy_init();
    if (!curl) { token->s_token = RC_CURL_INIT_FAILED; return NULL; }

    FILE* f = fopen(file, "w");

    if (f) {

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, f);

        rc_curl_auto_perform(token, curl);
        fclose(f);

        curl_easy_cleanup(curl);
        return file;

    } else {

        curl_easy_cleanup(curl);
        return NULL;

    }

}

const char* rc_json_fwrite(JsonContent* json, const char* file) {

    if (json->n_bytes == 0) { return NULL; }

    FILE* f = fopen(file, "w");

    if (f) {

        fwrite(json->buffer, 1, json->n_bytes, f);
        fflush(f);
        fclose(f);

        return file;

    } else { return NULL; }

}

void rc_json_free(JsonContent* json) { free(json->buffer); }
