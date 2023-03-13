#include <string.h>
#include <stdlib.h>

#include "server.h"
#include "parsers.h"


// Finds the first occurence of a character in a string
// and returns its index
int strfchar(char *str, char c) {
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] == c) return i;
    }
    return -1;
}

// Finds the first occurence of a character in a string
// and returns its index
int strlchar(char *str, char c) {
    for (int i = strlen(str) - 1; i >= 0; i--) {
        if (str[i] == c) return i;
    }
    return -1;
}

// Finds the start of Http Request body and returns 
// a pointer to the first charcter of the body
const char *findBodyStart(const char *const rawRequest) {
    int count = 0;
    for (size_t i = 1; i < strlen(rawRequest); i++) {
        // Search for double \r\n sequence in request
        if (rawRequest[i] == '\r' || rawRequest[i] == '\n') {
            if (rawRequest[i] == '\n' && rawRequest[i - 1] == '\r') count++;
        } else {
            count = 0;
        }

        // Return pointer to character immediately following 
        // the sequence if found and if the address of 
        // the character is not out of bounds for the raw request string
        if (count == 2 && (i + 1) < strlen(rawRequest)) return &rawRequest[i + 1];
    }

    // Return NULL pointer if no body found
    return NULL;
}

HttpMethods parseMethodStr(char *methodStr) {
    HttpMethods method;
    if (strcmp(methodStr, "GET") == 0) {
        method = GET;
    } else if (strcmp(methodStr, "POST") == 0) {
        method = POST;
    } else if (strcmp(methodStr, "PUT") == 0) {
        method = PUT;
    } else if (strcmp(methodStr, "DELETE") == 0) {
        method = DELETE;
    } else {
        method = INVALID;
    }
    return method;
}

HttpRequest parseRequest(const char *const rawRequest) {
    // Copy request string to another memory address to keep original unmodified
    char *buffer = malloc(strlen(rawRequest));
    memcpy(buffer, rawRequest, strlen(rawRequest));

    HttpRequest request;
    request.numOfHeaders = 0;

    // Get pointer to HTTP Method and copy to another memory location
    // while leaving original string unmodified to prevent memory leaks
    char *method = strtok(buffer, " ");
    // char *tempMethod = malloc(strlen(method));
    // memcpy(tempMethod, method, strlen(method));
    request.method = parseMethodStr(method);

    // Do the same thing for the url
    char *url = strtok(NULL, " ");
    char *tempUrl = malloc(strlen(url));
    memcpy(tempUrl, url, strlen(url));
    request.url = tempUrl;

    // TODO: implement parsing for headers

    // Get pointer to body 
    const char *body = findBodyStart(rawRequest);

    // check if there is a request body
    if (body == NULL) {
        request.body = NULL;
    } else {
        // copy request body to another memory loation if exists
        char *tempBody = malloc(strlen(body));
        memcpy(tempBody, body, strlen(body));
        request.body = tempBody;
    }

    return request;
}
