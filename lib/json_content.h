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

#ifndef RC_JSON_CONTENT_H
#define RC_JSON_CONTENT_H

#define JSON_INIT_SIZE (1 << 20)

#include "bearer_token.h"

/**
 * Not using opaque typedef here, specifically so that
 * RC_JSON_INIT can initialize the struct inline
 * without function call / copy on return
 */

/**
 * Container struct to store JSON response and facilitate page loop
 * Can be reused without freeing/re-initializing
 * 
 * -- Declaration & Initialization --
 * RIGHT: JsonContent* json = RC_JSON_INIT(0); 
 * WRONG: JsonContent* json; // this will cause a crash later.
 * 
 * -- Freeing Memory --
 * RIGHT: RC_JSON_FREE(json);
 * 
 * - Do not assume/directly modify its member variables or buffer
 * - Must be freed with RC_JSON_FREE when done
 */
typedef struct {

    char* buffer;
    size_t n_bytes;
    size_t n_pages;
    size_t n_chunk;

    const size_t init_size;
    size_t total_size;

    const char* url_next_page;

} JsonContent;

/// @brief Store JSON response in memory
/// @param token pointer to a BearerToken (can be just a skeleton)
/// @param json pointer to a JsonContent container
/// @param url full url
void rc_json_get_buffer(BearerToken* token, JsonContent* json, const char* url);

/// @brief Write JSON response directly to file without storing in memory
/// @attention This function does not support page loop. For paginated endpoints,
///            use rc_json_get_buffer, then (optionally) rc_json_fwrite instead.
/// @param token pointer to a BearerToken (can be just a skeleton)
/// @param file full path & file name to be written. If null, writing to stdout
/// @param url full url
/// @return if written successfully, the file name (same as the file argument);
///         otherwise, NULL
const char* rc_json_get_file(BearerToken* token, const char* file, const char* url);

/// @brief Write JsonContent buffer to file
/// @param json pointer to a JsonContent container
/// @param file full path & file name to be written. If null, writing to stdout
/// @return if written successfully, the file name (same as the file argument);
///         otherwise, NULL
const char* rc_json_fwrite(JsonContent* json, const char* file);

/// @brief Create and initialize a JsonContent container on the stack
/// @param X initial buffer size to reserve (default is 1 megabyte)
/// @return a pointer to the initialized JsonContent
/// @note To accept default reserve buffer size (1 megabyte), pass in 0;
///       Otherwise, pass in the desired reserve buffer size as a size_t
#define RC_JSON_INIT(X) &(JsonContent)       \
{                                            \
    .buffer = NULL,                          \
    .n_bytes = 0,                            \
    .n_pages = 0,                            \
    .n_chunk = 0,                            \
    .init_size = X > 0 ? X : JSON_INIT_SIZE, \
    .total_size = 0,                         \
    .url_next_page = NULL                    \
}

/// @brief Free a JsonContent's buffer
/// @param X pointer to a JsonContent container
#define RC_JSON_FREE(X) free((X)->buffer)

#endif
