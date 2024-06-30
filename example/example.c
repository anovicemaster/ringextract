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
    
    /**
     * Check the TokenError code after the rc_json_get_buffer transfer(s).
     * The only success code is RC_TOKEN_OK; any other code indicates a failure.
     * In case of failures, error messages are stored in the error buffer of token.
     * Print the error buffer the same way as printing any const char*
     */
    if (token->s_token != RC_TOKEN_OK) { printf("%s\n", token->error); }

    /**
     * If the transfer(s) succeeded, we can write the json contents to a file
     * by providing a pointer to JsonContent and a file name to rc_json_fwrite.
     * If the provided file name cannot be opened for writing, NULL is returned.
     * Otherwise, the contents are written and the same file name is returned.
     */
    else if (rc_json_fwrite(json, "ext.json") == NULL) { printf("FILE error\n"); }

    /**
     * If the file was written successfully, we print the number of bytes
     * written as a confirmation. That information is found in the n_bytes
     * member variable in each JsonContent struct.
     */
    else { printf("%lu bytes written\n", json->n_bytes); }

    RC_JSON_FREE(json); // free the JsonContent container

    /**
     * This is a one-liner that sends a GET request to the sites endpoint,
     * then directly prints the response payload to stdout without using a JsonContent.
     * It re-uses the token declared above.
     */
    rc_json_get_file(token, NULL, RC_DEFAULT_SITES);

    // Prints error message (if applicable), same as example above.
    if (token->s_token != RC_TOKEN_OK) { printf("%s\n", token->error); }

    // No need to free anything here. No memory allocation in rc_json_get_file.
    // Do not free the token. It is entirely stored on the stack.
    return 0;

}