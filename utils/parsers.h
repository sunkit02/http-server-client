#ifndef URLPARSER_H
#define URLPARSER_H

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "http.h"


// Common string operations
inline bool startsWith(char *start, char *str) {
    return strncmp(start, str, strlen(start)) == 0;
}


inline bool endsWith(char *end, char *str) {
    return strncmp(end, str + (strlen(str) - strlen(end)), strlen(end)) == 0;
}


inline bool contains(char *substr, char *str) {
    size_t n = strlen(str) - strlen(substr);
    for (size_t i = 0; i < n; i++) {
        if (startsWith(substr, str + i)) return true;
    }
    return false;
}


int strfchar(char *str, char c);

// Returns a pointer to the first character of the body of
// an HTTP request or response
const char *findBodyStart(const char *const rawRequest);


// Takes in the raw HTTP request string and returns a pointer a an
// HttpRequest struct containing the content of the raw request
HttpRequest *parseHttpRequest(const char *const rawRequest);


// Takes in an HttpRequest struct and returns a stringified
// representation of that HttpRequest
char *stringifyHttpRequest(HttpRequest *request);


// Takes in the raw HTTP response string and returns a pointer a an
// HttpResponse struct containing the content of the raw response.
// Returns the pointer to an HttpResponse struct if parsed successfully.
// Returns a NULL pointer if failed to parse the response.
HttpResponse *parseHttpResponse(const char *const rawResponse);

#endif // !DEBUG
