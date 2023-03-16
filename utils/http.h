#ifndef HTTP_H
#define HTTP_H

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
    // char *method;
    HttpMethods method;
    char *url;
    char *body;
    HttpHeader *headers;
    size_t numOfHeaders;
} HttpRequest;


typedef struct {
    char *url;
    bool canGet;
    bool canPost;
    bool canPut;
    bool canDelete;
} HttpEndPoint;


#endif // !HTTP_H


