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

#include <unistd.h>

#include "bearer_token.h"
#include "status.h"

extern TokenError rc_curl_set_token(BearerToken* token, CURL* curl);

static inline void rc_limiter_200_timeout(CURL* curl) {

#define RLA200 "x-rate-limit-remaining"
#define RLT200 "x-rate-limit-window"

    struct curl_header* header;
    uint64_t attempt = 0;
    uint64_t timeout = 0;

    if (curl_easy_header(curl, RLA200, 0, CURLH_HEADER, 0, &header) == CURLHE_OK)
    { attempt = strtoul(header->value, NULL, 10); }

    if (curl_easy_header(curl, RLT200, 0, CURLH_HEADER, 0, &header) == CURLHE_OK)
    { timeout = strtoul(header->value, NULL, 10); }

    if (attempt == 0) { sleep((unsigned int)timeout); }

#undef RLA200
#undef RLT200

}

static inline void rc_limiter_429_timeout(CURL* curl) {

    curl_off_t timeout;

    if (curl_easy_getinfo(curl, CURLINFO_RETRY_AFTER, &timeout) == CURLE_OK)
    { sleep((unsigned int)timeout); }

}

static inline void rc_limiter_503_timeout(uint64_t* timeout) {

    sleep((unsigned int)*timeout);
    *timeout <<= 1;

}

void rc_curl_set_limit(BearerToken* token, CURL* curl, uint64_t attempt, uint64_t timeout) {

    if (token->s_CURL != CURLE_OK && token->s_CURL != CURLE_HTTP_RETURNED_ERROR) {

        token->s_token = RC_CURL_TRANSFER_FAILED;
        return;

    } else { curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &token->s_HTTP); }

    switch (token->s_HTTP) {

    case HTTP_OK:
        rc_limiter_200_timeout(curl);
        return;

    case HTTP_TOO_MANY_REQUESTS:
        rc_limiter_429_timeout(curl);
        break;

    case HTTP_SERVICE_UNAVAILABLE: 
        rc_limiter_503_timeout(&timeout);
        break;
    
    default: // TODO: evaluate necessary fallback for other HTTP status codes
        token->s_token = RC_CURL_HTTP_STATUS;
        return;

    }

    if (attempt) {

        if (rc_curl_set_token(token, curl) == RC_TOKEN_OK) {

            token->s_CURL = curl_easy_perform(curl);
            rc_curl_set_limit(token, curl, attempt - 1, timeout);

        } else { /* defer to the error code set by rc_curl_set_token() */ }

    } else { token->s_token = RC_CURL_HTTP_STATUS; }
    
}
