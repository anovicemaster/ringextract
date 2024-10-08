
# RingEXtract

There are many existing RingCentral SDKs/libraries/packages covering various programming languages, but I have yet to find one in C/C++ so I'm providing my own. **This is not a full SDK, but an interface focused solely on data extraction from the RingEX API.**

### Goals:
- Provide the simplest possible interface that gets the job done
- Minimal dependencies (libcurl being the only one)
- Might as well optimize it since this is C

### Features:
- JWT flow implementation + access token management
- Built-in wait timeout + automatic retry mechanism
- Built-in page loop (for paginated JSON resources)
- URL presets for 40+ common endpoints

### Limitations:
- HTTP GET requests only
- JWT flow only
- No batch requests (endpoints suitable for bulk extraction typically do not support batch requests anyways)
- No multi-threading support yet - may add in the future
- Not compatible with Windows

### Dependencies:
- libcurl >= 7.84.0

### Known Issues:
- Not all HTTP error statuses are currently being handled. Only errors handled are 429 and 503.