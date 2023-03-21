#ifndef HTTP_H
#define HTTP_H

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

typedef enum {
    GET, POST, PUT, DELETE, INVALID
} HttpMethods;


typedef struct {
    char *key;
    char *value;
} HttpHeader;

typedef struct {
    HttpHeader **headers;
    size_t size;
    size_t capacity;
} HttpHeaderList;


typedef struct {
    // char *method;
    HttpMethods method;
    char *url;
    char *body;
    HttpHeader *headers;
    size_t numOfHeaders;
} HttpRequest;


typedef struct {
    int status;
    HttpHeader *headers;
    char *body;
} HttpResponse;


typedef struct {
    char *url;
    bool canGet;
    bool canPost;
    bool canPut;
    bool canDelete;
} HttpEndPoint;

// Constructs an empty HttpHeaderList.
// Returns NULL if failed to construct the list
HttpHeaderList *constructHttpHeaderList(size_t capacity);
// Adds to the end of the HttpHeaderList.
// Returns false if failed to add to list (failed to allocate memory for new entry)
bool httpHeaderListAdd(HttpHeaderList *list, char *key, char *value);
// Gets the HttpHeader in HttpHeaderList if key exists. 
// Returns NULL if key doesn't exist
HttpHeader *httpHeaderListGet(HttpHeaderList *list, char *key);
// Removes HttpHeader in list with specified key
// Returns true if removed successfully, false if key doesn't exist
// Note: Removing HttpHeader from list frees its memory
bool httpHeaderListRemove(HttpHeaderList *list, char *key);
// Checks if an HttpHeader with specified key exists in list
bool httpHeaderListContainsHeader(HttpHeaderList *list, char *key);


#endif // !HTTP_H


