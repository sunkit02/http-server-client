#include <string.h>
#include <stdlib.h>

#include "http.h"
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


HttpRequest *parseHttpRequest(const char *const rawRequest) {
    // Copy request string to another memory address to keep original unmodified
    char *buffer = malloc(strlen(rawRequest));
    memcpy(buffer, rawRequest, strlen(rawRequest));

    HttpRequest *request = malloc(sizeof(HttpRequest));
    request->numOfHeaders = 0;

    // Get pointer to HTTP Method and copy to another memory location
    // while leaving original string unmodified to prevent memory leaks
    char *method = strtok(buffer, " ");
    // char *tempMethod = malloc(strlen(method));
    // memcpy(tempMethod, method, strlen(method));
    request->method = parseMethodStr(method);

    // Do the same thing for the url
    char *url = strtok(NULL, " ");
    char *tempUrl = malloc(strlen(url));
    memcpy(tempUrl, url, strlen(url));
    request->url = tempUrl;

    //////////////////////////////////////////
    // TODO: implement parsing for headers ///
    //////////////////////////////////////////

    // Get pointer to body 
    const char *body = findBodyStart(rawRequest);

    // check if there is a request body
    if (body == NULL) {
        request->body = NULL;
    } else {
        // copy request body to another memory loation if exists
        char *tempBody = malloc(strlen(body));
        memcpy(tempBody, body, strlen(body));
        request->body = tempBody;
    }

    return request;
}


HttpResponse *parseHttpResponse(const char *const rawResponse) {
    HttpResponse *response = malloc(sizeof(HttpResponse));
    // Pointers and buffers needed while parsing
    char *responseSavePtr;
    char *linePtr;
    char *tempPtr;
    char lineBuffer[2048];

    // Copy rawResponse into a buffer to avoid mutating original string
    char responseCopy[strlen(rawResponse) + 1];
    strcpy(responseCopy, rawResponse);

    // Get the starting point of the body as reference
    // for the stopping point when parsing the headers
    const char *bodyPtr = findBodyStart(responseCopy);


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
        return NULL;
    }
    // Parse status
    tempPtr = strtok(NULL, " "); 
    response->status= malloc(strlen(tempPtr) + 1);
    if (response->status == NULL) return NULL;
    strcpy(response->status, tempPtr);


    // Parse headers
    
    // Allocate memory for headerlist
    HttpHeaderList *headerList = constructHttpHeaderList(5);
    // Ensure that a valid list has been allocated
    if (headerList == NULL) return NULL;

    // Continue to parse as headers until reaches 
    // the first character of the response body
    while ((linePtr = strtok_r(NULL, "\r\n", &responseSavePtr)) < bodyPtr) {
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
            return NULL;
        }
    }
    // Assign headerlist to response
    response->headerList = headerList;


    // Parse body
    // response->body = "<h1>This is the body!</h1>";
    response->body = malloc(strlen(bodyPtr) + 1);
    if (response->body == NULL) return NULL;
    strcpy(response->body, bodyPtr);

    return response;
}

// Returns a stringified representation of an HTTP request.
// The returned string needs to be manually freed
char *stringifyHttpRequest(HttpRequest *request) {
    // TODO: Make buffer dynamically sized
    char *requestStr = malloc(1024);
    char *tempBuffer = malloc(1024);

    // Method
    char *methodStr = stringifyHttpMethod(request->method);
    if (methodStr == NULL) return NULL;
    strcat(requestStr, methodStr);
    strcat(requestStr, " ");

    // Url
    strcat(requestStr, request->url);
    strcat(requestStr, " ");

    // HTTP version
    strcat(requestStr, "HTTP/1.1");
    strcat(requestStr, "\r\n");

    // Headers
    for (size_t i = 0; i < request->headerList->size; i++) {
        HttpHeader *header = request->headerList->headers[i];
        sprintf(tempBuffer, 
                "%s:%s\r\n", header->key, header->value);
        strcat(requestStr, tempBuffer);
    }
    // End of headers
    strcat(requestStr, "\r\n");

    // Body
    strcat(requestStr, request->body);
    strcat(requestStr, "\r\n");

    free(tempBuffer);
    return requestStr;
}
