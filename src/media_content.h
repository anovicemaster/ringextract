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

#ifndef RC_MEDIA_CONTENT_H
#define RC_MEDIA_CONTENT_H

#include "bearer_token.h"

#define MEDIA_INIT_SIZE (1 << 20)

/**
 * Not using opaque typedef here, specifically so that
 * RC_MEDIA_INIT can initialize the struct inline
 * without function call / copy on return
 */

/**
 * Container struct to store binary media (audio/mpeg, audio/wav, etc.)
 * Can be reused without freeing/re-initializing
 * 
 * -- Declaration & Initialization --
 * RIGHT: MediaContent* media = RC_MEDIA_INIT(0);
 * WRONG: MediaContent* media; // this will cause a crash later.
 * 
 * -- Freeing Memory --
 * RIGHT: RC_MEDIA_FREE(&media);
 * 
 * - Do not assume/directly modify its member variables or buffer
 * - Must be freed with RC_MEDIA_FREE when done
 */
typedef struct {

    uint8_t* buffer;
    size_t n_bytes;

    const size_t init_size;
    size_t total_size;

} MediaContent;

/// @brief Store binary media in memory
/// @param token pointer to a BearerToken (can be just a skeleton)
/// @param media pointer to a MediaContent container
/// @param url full url
void rc_media_get_buffer(BearerToken* token, MediaContent* media, const char* url);

/// @brief Write media response directly to file without storing in memory
/// @param token pointer to a BearerToken (can be just a skeleton)
/// @param file full path & file name to be written
/// @param url full url
/// @return if written successfully, the file name (same as the file argument);
///         otherwise, NULL
const char* rc_media_get_file(BearerToken* token, const char* file, const char* url);

/// @brief Write MediaContent buffer to file
/// @param media pointer to a MediaContent container
/// @param file full path & file name to be written
/// @return if written successfully, the file name (same as the file argument);
///         otherwise, NULL
const char* rc_media_fwrite(MediaContent* media, const char* file);

/// @brief Create and initialize a MediaContent container on the stack
/// @param X initial buffer size to reserve (default is 1 megabyte)
/// @return a pointer to the initialized MediaContent
/// @note To accept default reserve buffer size (1 megabyte), pass in 0;
///       Otherwise, pass in the desired reserve buffer size as a size_t
#define RC_MEDIA_INIT(X) &(MediaContent)      \
{                                             \
    .buffer = NULL,                           \
    .n_bytes = 0,                             \
    .init_size = X > 0 ? X : MEDIA_INIT_SIZE, \
    .total_size = 0                           \
}

/// @brief Free a MediaContent's buffer
/// @param X pointer to a MediaContent container
#define RC_MEDIA_FREE(X) free((X)->buffer)

#endif
