#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "game.h"
#include "http.h"
#include "parsers.h"

void printBytes(const void *obj, size_t size) {
    const unsigned char *bytes = obj;

    printf("[ ");
    for (size_t i = 0; i < size; i++) {
        printf("%02x ", bytes[i]);
    }
    puts("]");
}

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

// Determines whether a character is \r, \n, or an empty space
bool shouldTruncate(char c) {
    return c == ' ' || c == '\r' || c == '\n';
}

// Takes a string and returns a pointer to an address with 
// a copy of the string with all \n, \r, and spaces truncated.
// Returns a NULL pointer if fails to allocate memory for copied string.
char *trimEnd(const char *str) {
    if (str == NULL) return NULL;

    // Get length of string
    size_t strLen = strlen(str);

    // Get pointer to last character
    const char *endPtr = &str[strLen - 1];

    // Keep track of number of chars truncated
    size_t charsTruncated = 0;

    // Continue to mark chars as truncated while they should be
    while (endPtr > str && shouldTruncate(*endPtr)) {
        endPtr--;
        charsTruncated++;
    }

    // Allocate enough memory for a string to hold all 
    // remaining characters
    char *newStr = malloc(strLen - charsTruncated + 1);

    // Ensure pointer is valid
    if (newStr == NULL) return NULL;

    // Copy remaining characters to new string
    memcpy(newStr, str, strLen - charsTruncated);

    // End new string properly
    newStr[strLen - charsTruncated] = '\0';

    return newStr;
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

char *stringifyHttpMethod(HttpMethods method) {
    char *methodStr;

    switch (method) {
        case GET:
            methodStr = "GET";
            break;
        case POST:
            methodStr = "POST";
            break;
        case PUT:
            methodStr = "PUT";
            break;
        case DELETE:
            methodStr = "DELETE";
            break;
        default:
            methodStr = NULL;
    }

    return methodStr;
}


// Parses a raw HTTP request string and returns an HttpRequest
// struct.
// Returns NULL if failed to allocate memory for the HttpRequest
// struct or an error occurred during parsing.
HttpRequest *parseHttpRequest(const char *const rawRequest) {
    HttpRequest *request = calloc(1, sizeof(HttpRequest));

    char *requestSavePtr = NULL; // holds pointer between transitions between different strtok_r calls
    char *linePtr = NULL; // holds pointer to current line being processed
    char *tempPtr = NULL; // holds pointer for parsing within a line
    char lineBuffer[2048] = {0}; // holds copied bytes from line currently being processed
    const char *bodyPtr = NULL; // holds pointer to first char of body if exists
    const char *headersEndPtr = NULL; // holds pointer to end of headers section

    // Copy rawRequest into a buffer to avoid mutating original string
    char requestCopy[strlen(rawRequest) + 1];
    strcpy(requestCopy, rawRequest);


    // Get the starting point of the body as reference
    // for the stopping point when parsing the headers
    bodyPtr = findBodyStart(requestCopy);

    // check if there is a request body
    if (bodyPtr != NULL) {
        request->body = trimEnd(bodyPtr);
    }

    // Get the ending point of the header section
    // Equals start of body if body is present
    // Equals end of request if no body present
    headersEndPtr = bodyPtr != NULL ? bodyPtr : (requestCopy + strlen(rawRequest)); 

    // Parse the first line
    linePtr = strtok_r(requestCopy, "\r\n", &requestSavePtr);
    // Copy the line for further parsing
    strcpy(lineBuffer, linePtr);

    // Get pointer to HTTP Method and copy to another memory location
    // while leaving original string unmodified to prevent memory leaks
    char *method = strtok(lineBuffer, " ");
    request->method = parseMethodStr(method);

    // Do the same thing for the url
    char *url = strtok(NULL, " ");
    request->url = malloc(strlen(url) + 1);
    strcpy(request->url, url);

    // Ignore HTTP version

    // Allocate memory for headerlist
    HttpHeaderList *headerList = constructHttpHeaderList(5);
    // Ensure that a valid list has been allocated
    if (headerList == NULL) {
        httpRequestDestroy(request);
        return NULL;
    }

    // Continue to parse as headers until reaches 
    // the first character of the response body
    while ((linePtr = strtok_r(NULL, "\r\n", &requestSavePtr)) < headersEndPtr) {
        // Ensure that linePtr is valid
        if (linePtr == NULL) break;

        // Copy current line for further parsing
        strcpy(lineBuffer, linePtr);

        // Parse key of header
        tempPtr = strtok(lineBuffer, " : ");
        char tempKey[strlen(tempPtr) + 1];
        strcpy(tempKey, tempPtr);
        // Parse value of header
        tempPtr = strtok(NULL, " : ");
        char tempValue[strlen(tempPtr) + 1];
        strcpy(tempValue, tempPtr);

        // Add header to headerList
        if (!httpHeaderListAdd(headerList, tempKey, tempValue)) {
            httpRequestDestroy(request);
            return NULL;
        }
    }

    // Assign headerlist to response
    request->headerList = headerList;

    return request;
}

// Parses a raw HTTP response string and returns an HttpResponse
// struct.
// Returns NULL if failed to allocate memory for the HttpResponse
// struct or an error occurred during parsing.
HttpResponse *parseHttpResponse(const char *const rawResponse) {
    HttpResponse *response = calloc(1, sizeof(HttpResponse));

    char *responseSavePtr = NULL; // holds pointer between transitions between different strtok_r calls
    char *linePtr = NULL; // holds pointer to current line being processed
    char *tempPtr = NULL; // holds pointer for parsing within a line
    char lineBuffer[2048] = {0}; // holds copied bytes from line currently being processed
    const char *bodyPtr = NULL; // holds pointer to first char of body if exists
    const char *headersEndPtr = NULL; // holds pointer to end of headers section

    // Copy rawResponse into a buffer to avoid mutating original string
    char *responseCopy = malloc(strlen(rawResponse) + 1);
    strcpy(responseCopy, rawResponse);

    // Get the starting point of the body as reference
    // for the stopping point when parsing the headers
    bodyPtr = findBodyStart(responseCopy);

    // Parse body if exists
    if (bodyPtr != NULL) {
        response->body = trimEnd(bodyPtr);
    }

    // Get the ending point of the header section
    // Equals start of body if body is present
    // Equals end of request if no body present
    headersEndPtr = bodyPtr != NULL ? bodyPtr : (responseCopy + strlen(rawResponse)); 

    // Parse the first line
    linePtr = strtok_r(responseCopy, "\r\n", &responseSavePtr);
    // Copy the line for further parsing
    strcpy(lineBuffer, linePtr);

    // Ignore HTTP version
    strtok(lineBuffer, " ");
    // Parse status code 
    tempPtr = strtok(NULL, " "); 
    response->statusCode = atoi(tempPtr);
    if (response->statusCode == 0) {
        printf("Invalid status code: %s\n", tempPtr);
        httpResponseDestroy(response);
        return NULL;
    }
    // Parse status
    tempPtr = strtok(NULL, " "); 
    response->status= malloc(strlen(tempPtr) + 1);
    if (response->status == NULL) {
        httpResponseDestroy(response);
        return NULL;
    }
    strcpy(response->status, tempPtr);


    // Parse headers
    
    HttpHeaderList *headerList = constructHttpHeaderList(5);
    // Ensure that a valid list has been allocated
    if (headerList == NULL) {
        httpResponseDestroy(response);
        return NULL;
    }

    // Continue to parse as headers until reaches 
    // the first character of the response body
    while ((linePtr = strtok_r(NULL, "\r\n", &responseSavePtr)) < headersEndPtr) {
        // Ensure that linePtr is valid
        if (linePtr == NULL) break;

        // Copy current line for further parsing
        strcpy(lineBuffer, linePtr);
        
        // Parse key of header
        tempPtr = strtok(lineBuffer, " : ");
        char tempKey[strlen(tempPtr) + 1];
        strcpy(tempKey, tempPtr);
        // Parse value of header
        tempPtr = strtok(NULL, " : ");
        char tempValue[strlen(tempPtr) + 1];
        strcpy(tempValue, tempPtr);

        // Add header to headerList
        if (!httpHeaderListAdd(headerList, tempKey, tempValue)) {
            httpResponseDestroy(response);
            return NULL;
        }
    }

    // Assign headerlist to response
    response->headerList = headerList;

    free(responseCopy);
    return response;
}


HttpResponse *parseBinaryHttpResponse(const char *const rawResponse, size_t responseLength) {
    // FIX: Implement parsing for binary data (get length of entire rawRequest somehow)
    HttpResponse *response = calloc(1, sizeof(HttpResponse));

    char *responseSavePtr = NULL; // holds pointer between transitions between different strtok_r calls
    char *linePtr = NULL; // holds pointer to current line being processed
    char *tempPtr = NULL; // holds pointer for parsing within a line
    char lineBuffer[2048] = {0}; // holds copied bytes from line currently being processed
    const char *bodyPtr = NULL; // holds pointer to first char of body if exists
    const char *headersEndPtr = NULL; // holds pointer to end of headers section

    // Copy rawResponse into a buffer to avoid mutating original string
    char *responseCopy = malloc(responseLength + 1);
    memcpy(responseCopy, rawResponse, responseLength);
    responseCopy[responseLength] = '\0';

    // Get the starting point of the body as reference
    // for the stopping point when parsing the headers
    bodyPtr = findBodyStart(responseCopy);

    // Parse body if exists
    if (bodyPtr != NULL) {
        response->body = trimEnd(bodyPtr);
    }
    // if (bodyPtr != NULL) {
    //     if (response->headerList == NULL ||
    //         !httpHeaderListContainsHeader(response->headerList, "Content-Type")) {
    //             response->body = trimEnd(bodyPtr);
    //     } else {
    //         HttpHeader *contentTypeHeader =
    //             httpHeaderListGet(response->headerList, "Content-Type");
    //
    //         if (strcmp(contentTypeHeader->value, "binary") == 0) {
    //             char *bodySizeStr =
    //                 httpHeaderListGet(response->headerList, "Content-Length")->value;
    //
    //             size_t bodySize = strtoul(bodySizeStr, NULL, 10);
    //
    //             if ((response->body = malloc(bodySize)) == NULL) {
    //                 httpResponseDestroy(response);
    //                 return NULL;
    //             }
    //
    //             memcpy(response->body, bodyPtr, bodySize);
    //         } else {
    //             response->body = trimEnd(bodyPtr);
    //         }
    //     }
    // }

    // Get the ending point of the header section
    // Equals start of body if body is present
    // Equals end of request if no body present
    headersEndPtr = bodyPtr != NULL ? bodyPtr : (responseCopy + strlen(rawResponse)); 

    // Parse the first line
    linePtr = strtok_r(responseCopy, "\r\n", &responseSavePtr);
    // Copy the line for further parsing
    strcpy(lineBuffer, linePtr);

    // Ignore HTTP version
    strtok(lineBuffer, " ");
    // Parse status code 
    tempPtr = strtok(NULL, " "); 
    response->statusCode = atoi(tempPtr);
    if (response->statusCode == 0) {
        printf("Invalid status code: %s\n", tempPtr);
        httpResponseDestroy(response);
        return NULL;
    }
    // Parse status
    tempPtr = strtok(NULL, " "); 
    response->status= malloc(strlen(tempPtr) + 1);
    if (response->status == NULL) {
        httpResponseDestroy(response);
        return NULL;
    }
    strcpy(response->status, tempPtr);


    // Parse headers
    
    HttpHeaderList *headerList = constructHttpHeaderList(5);
    // Ensure that a valid list has been allocated
    if (headerList == NULL) {
        httpResponseDestroy(response);
        return NULL;
    }

    // Continue to parse as headers until reaches 
    // the first character of the response body
    while ((linePtr = strtok_r(NULL, "\r\n", &responseSavePtr)) < headersEndPtr) {
        // Ensure that linePtr is valid
        if (linePtr == NULL) break;

        // Copy current line for further parsing
        strcpy(lineBuffer, linePtr);
        
        // Parse key of header
        tempPtr = strtok(lineBuffer, " : ");
        char tempKey[strlen(tempPtr) + 1];
        strcpy(tempKey, tempPtr);
        // Parse value of header
        tempPtr = strtok(NULL, " : ");
        char tempValue[strlen(tempPtr) + 1];
        strcpy(tempValue, tempPtr);

        // Add header to headerList
        if (!httpHeaderListAdd(headerList, tempKey, tempValue)) {
            httpResponseDestroy(response);
            return NULL;
        }
    }

    // Assign headerlist to response
    response->headerList = headerList;

    free(responseCopy);
    return response;
}

// Returns a stringified representation of an HTTP request.
// Returns NULL if any essential fields are NULL (i.e. url).
// The returned string needs to be manually freed.
char *stringifyHttpRequest(HttpRequest *request) {
    // TODO: implement resizing when requestStr and tempBuffer run out of space
    char *requestStr = calloc(1024, sizeof(char));
    char *tempBuffer = calloc(1024, sizeof(char));

    // Method
    char *methodStr = stringifyHttpMethod(request->method);
    if (methodStr == NULL) return NULL;
    strcat(requestStr, methodStr);
    strcat(requestStr, " ");

    // Url
    if (request->url == NULL) return NULL;
    strcat(requestStr, request->url);
    strcat(requestStr, " ");

    // HTTP version
    strcat(requestStr, "HTTP/1.1");
    strcat(requestStr, "\r\n");

    // Headers
    if (request->headerList != NULL) {
        for (size_t i = 0; i < request->headerList->size; i++) {
            HttpHeader *header = request->headerList->headers[i];
            sprintf(tempBuffer, "%s:%s\r\n",
                    header->key, header->value);
            strcat(requestStr, tempBuffer);
        }
    }
    // End of headers
    strcat(requestStr, "\r\n");

    
    // Body
    if (request->body != NULL) {
        strcat(requestStr, request->body);
        strcat(requestStr, "\r\n");
    }
    // size_t requestStrLength = strlen(requestStr);
    // if (request->body != NULL) {
    //     HttpHeader *header = httpHeaderListGet(request->headerList, "Content-Type");
    //
    //     // Handle binary request body
    //     if (header != NULL && strcmp(header->value, "binary") == 0) {
    //         // TODO: Implement base64 encoding and fix finding strlen problem
    //         char *bodySizeStr = 
    //             httpHeaderListGet(request->headerList, "Content-Length")->value;
    //
    //         size_t bodySize = strtoul(bodySizeStr, NULL, 10);
    //         char *strEnd = &requestStr[strlen(requestStr)];
    //
    //         // Using memcpy instead of strcpy to avoid interpreting raw bytes
    //         // as control characters like '\0'
    //         memcpy(strEnd, request->body, bodySize);
    //         memcpy(strEnd + bodySize - 1, "\r\n", 3);
    //         // Populate stringifyMessage 
    //         // lengthBeforeAppending + sizeof(Player) + 4 terminating characters
    //         stringifiedMessage->length = requestStrLength + sizeof(Player) + 4;
    //         stringifiedMessage->message = requestStr;
    //
    //         requestStrLength += sizeof(Player) + 2;
    //     } else {
    //         // Directly copy as string if not binary
    //         strcat(requestStr, request->body);
    //
    //         // Check for '\r\n' at end of current request and append if not
    //         if (requestStr[strlen(requestStr) - 2] != '\r' &&
    //             requestStr[strlen(requestStr) - 1] != '\n') {
    //             strcat(requestStr, "\r\n");
    //         }
    //
    //         // Two extra characters for last terminating sequence
    //         stringifiedMessage->length = strlen(requestStr) + 2;
    //         stringifiedMessage->message = requestStr;
    //         requestStrLength = strlen(requestStr);
    //     }
    // }

    // Last terminating sequence
    strcat(requestStr, "\r\n");
    // memcpy(&requestStr[requestStrLength], "\r\n", 3);

    free(tempBuffer);
    return requestStr;
}

char *stringifyHttpResponse(HttpResponse *response) {
    // TODO: implement resizing when responseStr and tempBuffer run out of space
    char *responseStr = calloc(4096, sizeof(char));
    char *tempBuffer = calloc(1024, sizeof(char));

    // HTTP version
    strcat(responseStr, "HTTP/1.1 ");

    // Status code
    sprintf(tempBuffer, "%d ", response->statusCode);
    strcat(responseStr, tempBuffer);

    // Status
    strcat(responseStr, response->status);
    strcat(responseStr, "\r\n");

    // Headers
    if (response->headerList != NULL) {
        for (size_t i = 0; i < response->headerList->size; i++) {
            HttpHeader *header = response->headerList->headers[i];
            sprintf(tempBuffer, "%s: %s\r\n", 
                    header->key, header->value);
            strcat(responseStr, tempBuffer);
        }
    }
    // End of headers
    strcat(responseStr, "\r\n");

    // Body
    if (response->body != NULL) {
        strcat(responseStr, response->body);
        strcat(responseStr, "\r\n");
    }

    // Last terminating sequence
    strcat(responseStr, "\r\n");

    free(tempBuffer);
    return responseStr;
}


// Frees all memory used by HttpRequest
void httpRequestDestroy(HttpRequest *request) {
    if (request != NULL) {
        if (request->url != NULL)
            free(request->url);
        if (request->body != NULL)
            free(request->body);
        if (request->headerList != NULL)
            httpHeaderListDestroy(request->headerList);
        free(request);
    }
}


// Frees all memory used by HttpResponse
void httpResponseDestroy(HttpResponse *response) {
    if (response != NULL) {
        if (response->status != NULL)
            free(response->status);
        if (response->body != NULL)
            free(response->body);
        if (response->headerList != NULL)
            httpHeaderListDestroy(response->headerList);
    }
    free(response);
}

char *serializeObj(void *obj, size_t size) {
    // allocate memory for void buffer and extra bytes
    // for null terminating character
    char *serializedObj = malloc(sizeof(Player) + 1);
    // copy player bytes to void buffer
    memcpy(serializedObj, obj, size); 
    serializedObj[size] = '\0';

    // return void buffer
    return serializedObj;
}

void *deserializeObj(char *objByteString, size_t size) {
    void *deserializedObj = malloc(size); 
    memcpy(deserializedObj, objByteString, size);
    return deserializedObj;
}

