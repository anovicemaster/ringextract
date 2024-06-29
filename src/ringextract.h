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

#ifndef RINGEXTRACT_H
#define RINGEXTRACT_H

/**
 * The default options mostly conform to conventions used by existing SDKs
 * Default environment variable name for url: RC_SERVER_URL
 * Default environment variable name for client id: RC_CLIENT_ID
 * Default environment variable name for client secret: RC_CLIENT_SECRET
 * Default environment variable name for JSON Web Token: RC_JWT
 */

#ifndef RC_PRODUCTION
#define RC_SERVER_URL "https://platform.devtest.ringcentral.com"
#else
#define RC_SERVER_URL "https://platform.ringcentral.com"
#endif // RC_PRODUCTION

#ifndef RC_CLIENT_ID
#define RC_CLIENT_ID getenv("RC_CLIENT_ID")
#endif // RC_CLIENT_ID

#ifndef RC_CLIENT_SECRET
#define RC_CLIENT_SECRET getenv("RC_CLIENT_SECRET")
#endif // RC_CLIENT_SECRET

#ifndef RC_JWT
#define RC_JWT getenv("RC_JWT")
#endif // RC_JWT

#ifndef RC_DEFAULT_DISABLE
#define RC_ENDPOINT(X) RC_SERVER_URL X

#define RC_ENDPOINT_TOKEN        RC_ENDPOINT("/restapi/oauth/token")
#define RC_ENDPOINT_REVOKE       RC_ENDPOINT("/restapi/oauth/revoke")
#define RC_ENDPOINT_SITES        RC_ENDPOINT("/restapi/v1.0/account/~/sites")
#define RC_ENDPOINT_CALL_LOG     RC_ENDPOINT("/restapi/v1.0/account/~/call-log")
#define RC_ENDPOINT_ACTIVE_CALLS RC_ENDPOINT("/restapi/v1.0/account/~/active-calls")
#define RC_ENDPOINT_CALL_QUEUES  RC_ENDPOINT("/restapi/v1.0/account/~/call-queues")
#define RC_ENDPOINT_MEMBERS      RC_ENDPOINT("/restapi/v1.0/account/~/call-queues/%li/members")
#define RC_ENDPOINT_PHONE_NUMBER RC_ENDPOINT("/restapi/v1.0/account/~/phone-number")
#define RC_ENDPOINT_EXTENSION    RC_ENDPOINT("/restapi/v1.0/account/~/extension")
#define RC_ENDPOINT_LANGUAGE     RC_ENDPOINT("/restapi/v1.0/dictionary/language")
#define RC_ENDPOINT_COUNTRY      RC_ENDPOINT("/restapi/v1.0/dictionary/country")
#define RC_ENDPOINT_LOCATION     RC_ENDPOINT("/restapi/v1.0/dictionary/location")
#define RC_ENDPOINT_STATE        RC_ENDPOINT("/restapi/v1.0/dictionary/state")
#define RC_ENDPOINT_TIMEZONE     RC_ENDPOINT("/restapi/v1.0/dictionary/timezone")

#define RC_SET_QUERY(X, Y) X "?" Y

#define RC_DEFAULT_SITES        RC_SET_QUERY(RC_ENDPOINT_SITES,        "perPage=1000")
#define RC_DEFAULT_CALL_LOG     RC_SET_QUERY(RC_ENDPOINT_CALL_LOG,     "perPage=1000&view=Detailed")
#define RC_DEFAULT_ACTIVE_CALLS RC_SET_QUERY(RC_ENDPOINT_ACTIVE_CALLS, "perPage=1000&view=Detailed")
#define RC_DEFAULT_CALL_QUEUES  RC_SET_QUERY(RC_ENDPOINT_CALL_QUEUES,  "perPage=1000")
#define RC_DEFAULT_MEMBERS      RC_SET_QUERY(RC_ENDPOINT_MEMBERS,      "perPage=1000")
#define RC_DEFAULT_PHONE_NUMBER RC_SET_QUERY(RC_ENDPOINT_PHONE_NUMBER, "perPage=1000")
#define RC_DEFAULT_EXTENSION    RC_SET_QUERY(RC_ENDPOINT_EXTENSION,    "perPage=1000")
#define RC_DEFAULT_LANGUAGE     RC_SET_QUERY(RC_ENDPOINT_LANGUAGE,     "perPage=1000")
#define RC_DEFAULT_COUNTRY      RC_SET_QUERY(RC_ENDPOINT_COUNTRY,      "perPage=1000")
#define RC_DEFAULT_LOCATION     RC_SET_QUERY(RC_ENDPOINT_LOCATION,     "perPage=1000&stateId=%li")
#define RC_DEFAULT_STATE        RC_SET_QUERY(RC_ENDPOINT_STATE,        "perPage=1000&allCountries=True")
#define RC_DEFAULT_TIMEZONE     RC_SET_QUERY(RC_ENDPOINT_TIMEZONE,     "perPage=1000")
#endif // RC_DEFAULT_DISABLE

#include "json_content.h"
#include "media_content.h"

#endif