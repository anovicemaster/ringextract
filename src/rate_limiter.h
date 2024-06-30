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

#ifndef RC_RATE_LIMITER_H
#define RC_RATE_LIMITER_H

#define MAX_RETRY_ATTEMPT 5
#define MIN_RETRY_TIMEOUT 15

#include "bearer_token.h"

typedef enum {

    HTTP_CONTINUE                        = 100, // [RFC9110, Section 15.2.1]
    HTTP_SWITCHING_PROTOCOLS             = 101, // [RFC9110, Section 15.2.2]
    HTTP_PROCESSING                      = 102, // [RFC2518]
    HTTP_EARLY_HINTS                     = 103, // [RFC8297]
    HTTP_OK                              = 200, // [RFC9110, Section 15.3.1]
    HTTP_CREATED                         = 201, // [RFC9110, Section 15.3.2]
    HTTP_ACCEPTED                        = 202, // [RFC9110, Section 15.3.3]
    HTTP_NON_AUTHORITATIVE_INFORMATION   = 203, // [RFC9110, Section 15.3.4]
    HTTP_NO_CONTENT                      = 204, // [RFC9110, Section 15.3.5]
    HTTP_RESET_CONTENT                   = 205, // [RFC9110, Section 15.3.6]
    HTTP_PARTIAL_CONTENT                 = 206, // [RFC9110, Section 15.3.7]
    HTTP_MULTI_STATUS                    = 207, // [RFC4918]
    HTTP_ALREADY_REPORTED                = 208, // [RFC5842]
    HTTP_IM_USED                         = 226, // [RFC3229]
    HTTP_MULTIPLE_CHOICES                = 300, // [RFC9110, Section 15.4.1]
    HTTP_MOVED_PERMANENTLY               = 301, // [RFC9110, Section 15.4.2]
    HTTP_FOUND                           = 302, // [RFC9110, Section 15.4.3]
    HTTP_SEE_OTHER                       = 303, // [RFC9110, Section 15.4.4]
    HTTP_NOT_MODIFIED                    = 304, // [RFC9110, Section 15.4.5]
    HTTP_USE_PROXY                       = 305, // [RFC9110, Section 15.4.6]
    HTTP_UNUSED_306                      = 306, // [RFC9110, Section 15.4.7]
    HTTP_TEMPORARY_REDIRECT              = 307, // [RFC9110, Section 15.4.8]
    HTTP_PERMANENT_REDIRECT              = 308, // [RFC9110, Section 15.4.9]
    HTTP_BAD_REQUEST                     = 400, // [RFC9110, Section 15.5.1]
    HTTP_UNAUTHORIZED                    = 401, // [RFC9110, Section 15.5.2]
    HTTP_PAYMENT_REQUIRED                = 402, // [RFC9110, Section 15.5.3]
    HTTP_FORBIDDEN                       = 403, // [RFC9110, Section 15.5.4]
    HTTP_NOT_FOUND                       = 404, // [RFC9110, Section 15.5.5]
    HTTP_METHOD_NOT_ALLOWED              = 405, // [RFC9110, Section 15.5.6]
    HTTP_NOT_ACCEPTABLE                  = 406, // [RFC9110, Section 15.5.7]
    HTTP_PROXY_AUTHENTICATION_REQUIRED   = 407, // [RFC9110, Section 15.5.8]
    HTTP_REQUEST_TIMEOUT                 = 408, // [RFC9110, Section 15.5.9]
    HTTP_CONFLICT                        = 409, // [RFC9110, Section 15.5.10]
    HTTP_GONE                            = 410, // [RFC9110, Section 15.5.11]
    HTTP_LENGTH_REQUIRED                 = 411, // [RFC9110, Section 15.5.12]
    HTTP_PRECONDITION_FAILED             = 412, // [RFC9110, Section 15.5.13]
    HTTP_CONTENT_TOO_LARGE               = 413, // [RFC9110, Section 15.5.14]
    HTTP_URI_TOO_LONG                    = 414, // [RFC9110, Section 15.5.15]
    HTTP_UNSUPPORTED_MEDIA_TYPE          = 415, // [RFC9110, Section 15.5.16]
    HTTP_RANGE_NOT_SATISFIABLE           = 416, // [RFC9110, Section 15.5.17]
    HTTP_EXPECTATION_FAILED              = 417, // [RFC9110, Section 15.5.18]
    HTTP_UNUSED_418                      = 418, // [RFC9110, Section 15.5.19]
    HTTP_MISDIRECTED_REQUEST             = 421, // [RFC9110, Section 15.5.20]
    HTTP_UNPROCESSABLE_CONTENT           = 422, // [RFC9110, Section 15.5.21]
    HTTP_LOCKED                          = 423, // [RFC4918]
    HTTP_FAILED_DEPENDENCY               = 424, // [RFC4918]
    HTTP_TOO_EARLY                       = 425, // [RFC8470]
    HTTP_UPGRADE_REQUIRED                = 426, // [RFC9110, Section 15.5.22]
    HTTP_PRECONDITION_REQUIRED           = 428, // [RFC6585]
    HTTP_TOO_MANY_REQUESTS               = 429, // [RFC6585]
    HTTP_REQUEST_HEADER_FIELDS_TOO_LARGE = 431, // [RFC6585]
    HTTP_UNAVAILABLE_FOR_LEGAL_REASONS   = 451, // [RFC7725]
    HTTP_INTERNAL_SERVER_ERROR           = 500, // [RFC9110, Section 15.6.1]
    HTTP_NOT_IMPLEMENTED                 = 501, // [RFC9110, Section 15.6.2]
    HTTP_BAD_GATEWAY                     = 502, // [RFC9110, Section 15.6.3]
    HTTP_SERVICE_UNAVAILABLE             = 503, // [RFC9110, Section 15.6.4]
    HTTP_GATEWAY_TIMEOUT                 = 504, // [RFC9110, Section 15.6.5]
    HTTP_HTTP_VERSION_NOT_SUPPORTED      = 505, // [RFC9110, Section 15.6.6]
    HTTP_VARIANT_ALSO_NEGOTIATES         = 506, // [RFC2295]
    HTTP_INSUFFICIENT_STORAGE            = 507, // [RFC4918]
    HTTP_LOOP_DETECTED                   = 508, // [RFC5842]
    HTTP_NOT_EXTENDED_OBSOLETED          = 510, // [RFC2774][status-change-http-experiments-to-historic]
    HTTP_NETWORK_AUTHENTICATION_REQUIRED = 511  // [RFC6585]

} HTTPcode;

/// @brief implementation of recursive retry/timeout mechanism
/// @param token pointer to a BearerToken struct
/// @param curl a CURL handle
/// @param attempt maximum retry attempts
/// @param timeout minimum retry timeout (in seconds)
void rc_curl_set_limit(BearerToken* token, CURL* curl, uint64_t attempt, uint64_t timeout);

#endif