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


// By default, RingEXtract uses the sandbox host: platform.devtest.ringcentral.com
// To switch to production host, simply define RC_PRODUCTION as below
// #define RC_PRODUCTION

// If your credentials are stored under non-standard environment variable names,
// uncomment the line(s) below and set the custom names inside getenv();
// Alternatively, you may replace each getenv() with a hard-coded string literal.
// Entering credentials as string literals is strongly discouraged in production.

// #define RC_CLIENT_ID getenv("MY_CLIENT_ID")
// #define RC_CLIENT_SECRET getenv("MY_CLIENT_SECRET")
// #define RC_JWT getenv("MY_JWT")

#include <stdio.h>
#include "ringextract.h"

/**
 * This program authenticates using JWT credentials,
 * sends a GET request to RingEX's extension endpoint,
 * retrieves all extensions in the account (can be 1, hundreds or tens of thousands),
 * writes the completed JSON response to "ext.json" in the current working directory,
 * and prints to stdout the size (number of bytes) of the json file written.
 * 
 * In the event that an error occurs, the appropriate error message is printed instead.
 */
int main(void) {

    /**
     * Initializing a token skeleton
     * The pointer can be passed to any data fetching API,
     * and reused throughout program execution.
     * 
     * Tokens are allocated on the stack by default.
     * Do not call free() on this pointer.
     */
    BearerToken* token = RC_TOKEN_SKELETON();

    /**
     * Initializing a JsonContent container
     * The container can be reused throughout program execution.
     * 
     * Must be freed by calling RC_JSON_FREE when done.
     */
    JsonContent* json = RC_JSON_INIT(0);

    /**
     * GET request to the extension endpoint,
     * then store the response payload in memory in the JsonContent container
     */
    rc_json_get_buffer(token, json, RC_DEFAULT_EXTENSION);
    
    if (token->s_token != RC_TOKEN_OK) { printf("HTTP error: %i\n", token->s_HTTP); }
    else if (rc_json_fwrite(json, "ext.json") == NULL) { printf("FILE error\n"); }
    else { printf("%lu bytes written\n", json->n_bytes); }

    RC_JSON_FREE(json); // free the JsonContent container

    return 0;

}