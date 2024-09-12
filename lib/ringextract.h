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

// Authentication - OAuth 2.0
#define RC_OAUTH_TOKEN   RC_SERVER_URL "/restapi/oauth/token"
#define RC_OAUTH_REVOKE  RC_SERVER_URL "/restapi/oauth/revoke"
#define RC_OAUTH_WSTOKEN RC_SERVER_URL "/restapi/oauth/wstoken"

/**
 * All endpoints/URLs defined below are provided solely for convenience.
 * If desired, setting RC_V1_DISABLE will remove all default definitions.
 * Data fetching APIs will always accept custom URLs regardless.
 * 
 * Certain endpoints require additional argument(s) as path/query parameter.
 * All definitions below can be used as format strings inside a formatting
 * function, such as snprintf, to easily generate the full url.
 * For example, to get a list of members for call queue id 23450001:
 * 
 * BearerToken* token = RC_TOKEN_SKELETON();
 * 
 * char url[128] = {0}; // set up a 128-byte buffer for the full url
 * snprintf(url, 128, RC_GET_CALL_QUEUE_MEMBERS, 23450001);
 * rc_json_get_file(token, NULL, url);
 * 
 */
#ifndef RC_V1_DISABLE
#define RC_V1_ACCT(X) RC_SERVER_URL "/restapi/v1.0/account/~"  X
#define RC_V1_DICT(X) RC_SERVER_URL "/restapi/v1.0/dictionary" X

/**
 * RC_V1_ macro set: basic string literals for each endpoint
 * 
 * Most endpoints can be used as-is without any query parameters;
 * However, adding appropriate query parameters can provide significant
 * performance benefits and/or granularity in the result dataset.
 * See RC_GET_ macros below for details.
 */

// Voice
#define RC_V1_BUSINESS_HOURS      RC_V1_ACCT("/business-hours")
#define RC_V1_ANSWERING_RULE      RC_V1_ACCT("/answering-rule")
#define RC_V1_FORWARD_ALL_CALLS   RC_V1_ACCT("/forward-all-calls")
#define RC_V1_CALL_LOG            RC_V1_ACCT("/call-log")
#define RC_V1_ACTIVE_CALLS        RC_V1_ACCT("/active-calls")
#define RC_V1_CALL_GROUPS         RC_V1_ACCT("/call-monitoring-groups")
#define RC_V1_CALL_GROUP_MEMBERS  RC_V1_ACCT("/call-monitoring-groups/%li/members")
#define RC_V1_CALL_QUEUES         RC_V1_ACCT("/call-queues")
#define RC_V1_CALL_QUEUE_MEMBERS  RC_V1_ACCT("/call-queues/%li/members")
#define RC_V1_CALL_RECORDING      RC_V1_ACCT("/call-recording")
#define RC_V1_CALL_RECORDING_EXT  RC_V1_ACCT("/call-recording/extensions")
#define RC_V1_CUSTOM_GREETINGS    RC_V1_ACCT("/call-recording/custom-greetings")
#define RC_V1_GREETING            RC_V1_DICT("/greeting")
#define RC_V1_IVR_PROMPTS         RC_V1_ACCT("/ivr-prompts")
#define RC_V1_IVR_MENUS           RC_V1_ACCT("/ivr-menus")

// Account
#define RC_V1_ACCOUNT             RC_V1_ACCT()
#define RC_V1_BUSINESS_ADDRESS    RC_V1_ACCT("/business-address")
#define RC_V1_SERVICE_INFO        RC_V1_ACCT("/service-info")
#define RC_V1_CUSTOM_FIELDS       RC_V1_ACCT("/custom-fields")
#define RC_V1_SITES               RC_V1_ACCT("/sites")
#define RC_V1_SITE_MEMBERS        RC_V1_ACCT("/sites/%li/members")
#define RC_V1_SITE_IVR            RC_V1_ACCT("/sites/%li/ivr")
#define RC_V1_PHONE_NUMBER        RC_V1_ACCT("/phone-number")
#define RC_V1_PRESENCE            RC_V1_ACCT("/presence")
#define RC_V1_CALL_QUEUE_PRESENCE RC_V1_ACCT("/call-queues/%li/presence")
#define RC_V1_LANGUAGE            RC_V1_DICT("/language")
#define RC_V1_COUNTRY             RC_V1_DICT("/country")
#define RC_V1_LOCATION            RC_V1_DICT("/location")
#define RC_V1_STATE               RC_V1_DICT("/state")
#define RC_V1_TIMEZONE            RC_V1_DICT("/timezone")
#define RC_V1_PERMISSION          RC_V1_DICT("/permission")
#define RC_V1_PERMISSION_CATEGORY RC_V1_DICT("/permission-category")

// Provisioning
#define RC_V1_USERS               RC_V1_ACCT("/emergency-address-auto-update/users")
#define RC_V1_WIRELESS_POINTS     RC_V1_ACCT("/emergency-address-auto-update/wireless-points")
#define RC_V1_NETWORKS            RC_V1_ACCT("/emergency-address-auto-update/networks")
#define RC_V1_DEVICES             RC_V1_ACCT("/emergency-address-auto-update/devices")
#define RC_V1_SWITCHES            RC_V1_ACCT("/emergency-address-auto-update/switches")
#define RC_V1_EMERGENCY_LOCATIONS RC_V1_ACCT("/emergency-locations")
#define RC_V1_EXTENSION           RC_V1_ACCT("/extension")
#define RC_V1_TEMPLATES           RC_V1_ACCT("/templates")

// Roles and Permissions
#define RC_V1_ASSIGNED_ROLE       RC_V1_ACCT("/assigned-role")
#define RC_V1_USER_ROLE           RC_V1_ACCT("/user-role")
#define RC_V1_USER_ROLE_DEFAULT   RC_V1_ACCT("/user-role/default")

/**
 * RC_GET_ macro set: full URLs with pre-defined query parameters
 * 
 * One of the most important query parameters for RingCentral data
 * extraction is "perPage=1000". By default, RingCentral's page size
 * is 100 (records per page) if no query parameter is provided.
 * This default setting is too restrictive for endpoints that return a
 * large number of records, most notably the call log.
 * The maximum permitted/effetive page size is 1000, and that's what
 * the presets below will use, wherever applicable.
 */

// Voice
#define RC_GET_BUSINESS_HOURS      RC_V1_BUSINESS_HOURS
#define RC_GET_ANSWERING_RULE      RC_V1_ANSWERING_RULE      "?perPage=1000"
#define RC_GET_FORWARD_ALL_CALLS   RC_V1_FORWARD_ALL_CALLS
#define RC_GET_CALL_LOG            RC_V1_CALL_LOG            "?perPage=1000&view=Detailed"
#define RC_GET_ACTIVE_CALLS        RC_V1_ACTIVE_CALLS        "?perPage=1000&view=Detailed"
#define RC_GET_CALL_GROUPS         RC_V1_CALL_GROUPS         "?perPage=1000"
#define RC_GET_CALL_GROUP_MEMBERS  RC_V1_CALL_GROUP_MEMBERS  "?perPage=1000"
#define RC_GET_CALL_QUEUES         RC_V1_CALL_QUEUES         "?perPage=1000"
#define RC_GET_CALL_QUEUE_MEMBERS  RC_V1_CALL_QUEUE_MEMBERS  "?perPage=1000"
#define RC_GET_CALL_RECORDING      RC_V1_CALL_RECORDING
#define RC_GET_CALL_RECORDING_EXT  RC_V1_CALL_RECORDING_EXT
#define RC_GET_CUSTOM_GREETINGS    RC_V1_CUSTOM_GREETINGS
#define RC_GET_GREETING            RC_V1_GREETING            "?perPage=1000"
#define RC_GET_IVR_PROMPTS         RC_V1_IVR_PROMPTS         "?perPage=1000"
#define RC_GET_IVR_MENUS           RC_V1_IVR_MENUS

// Account
#define RC_GET_ACCOUNT             RC_V1_ACCOUNT
#define RC_GET_BUSINESS_ADDRESS    RC_V1_BUSINESS_ADDRESS
#define RC_GET_SERVICE_INFO        RC_V1_SERVICE_INFO
#define RC_GET_CUSTOM_FIELDS       RC_V1_CUSTOM_FIELDS
#define RC_GET_SITES               RC_V1_SITES               "?perPage=1000"
#define RC_GET_SITE_MEMBERS        RC_V1_SITE_MEMBERS
#define RC_GET_SITE_IVR            RC_V1_SITE_IVR
#define RC_GET_PHONE_NUMBER        RC_V1_PHONE_NUMBER        "?perPage=1000"
#define RC_GET_PRESENCE            RC_V1_PRESENCE            "?perPage=1000"
#define RC_GET_CALL_QUEUE_PRESENCE RC_V1_CALL_QUEUE_PRESENCE
#define RC_GET_LANGUAGE            RC_V1_LANGUAGE            "?perPage=1000"
#define RC_GET_COUNTRY             RC_V1_COUNTRY             "?perPage=1000"
#define RC_GET_LOCATION            RC_V1_LOCATION            "?perPage=1000&stateId=%li"
#define RC_GET_STATE               RC_V1_STATE               "?perPage=1000&allCountries=True"
#define RC_GET_TIMEZONE            RC_V1_TIMEZONE            "?perPage=1000"
#define RC_GET_PERMISSION          RC_V1_PERMISSION          "?perPage=1000"
#define RC_GET_PERMISSION_CATEGORY RC_V1_PERMISSION_CATEGORY "?perPage=1000"

// Provisioning
#define RC_GET_USERS               RC_V1_USERS               "?perPage=1000"
#define RC_GET_WIRELESS_POINTS     RC_V1_WIRELESS_POINTS     "?perPage=1000"
#define RC_GET_NETWORKS            RC_V1_NETWORKS            "?perPage=1000"
#define RC_GET_DEVICES             RC_V1_DEVICES             "?perPage=1000"
#define RC_GET_SWITCHES            RC_V1_SWITCHES            "?perPage=1000"
#define RC_GET_EMERGENCY_LOCATIONS RC_V1_EMERGENCY_LOCATIONS "?perPage=1000"
#define RC_GET_EXTENSION           RC_V1_EXTENSION           "?perPage=1000"
#define RC_GET_TEMPLATES           RC_V1_TEMPLATES           "?perPage=1000"

// Roles and Permissions
#define RC_GET_ASSIGNED_ROLE       RC_V1_ASSIGNED_ROLE       "?perPage=1000"
#define RC_GET_USER_ROLE           RC_V1_USER_ROLE           "?perPage=1000"
#define RC_GET_USER_ROLE_DEFAULT   RC_V1_USER_ROLE_DEFAULT
#endif // RC_V1_DISABLE

/**
 * Currently, only the v1 family of endpoints are supported and tested.
 * Support for v2 may be added in the future.
 */

#include "json_content.h"
#include "media_content.h"

#endif // RINGEXTRACT_H
