
# RingEX C Interface for Data Extraction

### Warning: This repository is provided for educational and/or development purposes ONLY. Use in production AT YOUR OWN RISK.

### Goals:
- Provide the simplest possible interface that gets the job done
- Minimal dependencies (libcurl being the only one)
- Might as well optimize it since this is C

### Features:
- JWT flow implementation + access token management
- Built-in wait timeout + automatic retry mechanism
- Built-in page loop (for paginated JSON resources)
- URL presets for some common endpoints (call-log, phone-number, extension, etc.)

### Limitations:
- HTTP GET requests only
- JWT flow only
- No batch requests (endpoints suitable for bulk extraction typically do not support batch requests anyways)
- Limited testing performed over selected endpoints
- Not compatible with Windows

### Dependencies:
- libcurl >= 7.84.0

### Known Issues:
- Not all HTTP error statuses are currently being handled. Only errors handled are 429 and 503.