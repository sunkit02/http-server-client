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
const char *findBodyStart(const char *const rawRequest);
HttpRequest parseRequest(const char *const rawRequest);

#endif // !DEBUG
