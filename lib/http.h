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
    HttpMethods method;
    char *url;
    char *body;
    HttpHeaderList *headerList;
} HttpRequest;


typedef struct {
    int statusCode;
    char *status;
    char *body;
    HttpHeaderList *headerList;
} HttpResponse;


typedef struct {
    char *url;
    void (*callbacks[4])(int clientSocket, HttpRequest *request);
} HttpEndPoint;

// Constructs an empty HttpHeaderList.
// Returns NULL if failed to construct the list.
// NOTE:HttpHeaderList returned by this function needs to be manually freed
HttpHeaderList *constructHttpHeaderList(size_t capacity);


// Adds to the end of the HttpHeaderList.
// Returns false if failed to add to list (failed to allocate memory for new entry).
bool httpHeaderListAdd(HttpHeaderList *list, char *key, char *value);


// Gets the HttpHeader in HttpHeaderList if key exists.
// Returns NULL if key doesn't exist.
HttpHeader *httpHeaderListGet(HttpHeaderList *list, char *key);


// Removes HttpHeader in list with specified key.
// Returns true if removed successfully, false if key doesn't exist.
// NOTE: Removing HttpHeader from list frees its memory.
bool httpHeaderListRemove(HttpHeaderList *list, char *key);


// Checks if an HttpHeader with specified key exists in list
bool httpHeaderListContainsHeader(HttpHeaderList *list, char *key);

// Frees all memory used by HttpHeader
void httpHeaderDestroy(HttpHeader *header);


// Frees all memory used by HttpHeaderList
void httpHeaderListDestroy(HttpHeaderList *list);


// Constructs an HttpRequest based on method, url, headerList, and body passed in.
// NOTE:HttpRequest returned by this function needs to be manually freed.
// NOTE:Freeing the HttpHeaderList passed in here manually can lead to undefined behavior.
HttpRequest *constructHttpRequest(HttpMethods method, char *url, 
                                  HttpHeaderList *headerList,char *body);

// Constructs an HttpResponse based on statusCode, body, and headerList passed in.
// NOTE:HttpResponse returned by this function needs to be manually freed.
// NOTE:Freeing the HttpHeaderList passed in here manually can lead to undefined behavior.
HttpResponse *constructHttpResponse(int statusCode,
                                    HttpHeaderList *headerList,
                                    char *body);

// Frees all memory used by HttpRequest
void httpRequestDestroy(HttpRequest *request);

// Frees all memory used by HttpResponse
void httpResponseDestroy(HttpResponse *response);


#endif // !HTTP_H
