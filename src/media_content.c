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
#include "media_content.h"

#define MUL(X, Y) (((X) / (Y) + 1) * (Y))

static size_t rc_curl_write_media(char* contents, size_t size, size_t nitems, void* userdata) {

    MediaContent* media = (MediaContent*)userdata;
    const size_t chunk_size = size * nitems;
    const size_t old_size = media->n_bytes;

    if (!chunk_size) { return 0; }
    else { media->n_bytes += chunk_size; }

    if (media->n_bytes > media->total_size) {

        media->total_size = MUL(media->n_bytes, media->init_size);
        uint8_t* buffer = realloc(media->buffer, media->total_size);

        if (buffer) { media->buffer = buffer; }
        else { return 0; }

    }

    memcpy(media->buffer + old_size, contents, chunk_size);
    return chunk_size;

}

static inline void rc_media_reset(MediaContent* media) { media->n_bytes = 0; }

void rc_media_get_buffer(BearerToken* token, MediaContent* media, const char* url) {

    CURL* curl = curl_easy_init();

    if (curl) {

        rc_media_reset(media);
    
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, rc_curl_write_media);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, media);

        rc_curl_auto_perform(token, curl);
        curl_easy_cleanup(curl);

    } else { token->s_token = RC_CURL_INIT_FAILED; }

}

const char* rc_media_get_file(BearerToken* token, const char* file, const char* url) {

    CURL* curl = curl_easy_init();
    if (!curl) { token->s_token = RC_CURL_INIT_FAILED; return NULL; }

    FILE* f = fopen(file, "wb");

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

const char* rc_media_fwrite(MediaContent* media, const char* file) {

    if (media->n_bytes == 0) { return NULL; }

    FILE* f = fopen(file, "wb");

    if (f) {

        fwrite(media->buffer, 1, media->n_bytes, f);
        fflush(f);
        fclose(f);

        return file;

    } else { return NULL; }

}

void rc_media_free(MediaContent* media) { free(media->buffer); }
