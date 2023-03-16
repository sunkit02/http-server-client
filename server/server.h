#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

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


typedef struct {
    HttpEndPoint *endpoints;
    size_t size;
    size_t capacity;
} EndpointList;


typedef struct server {
    int domain;
    int service;
    int protocol;
    u_long interface;
    int port;
    int backlog;
    struct sockaddr_in address;
    int socket;
    EndpointList *endpointList;
    void (*launch)(struct server *);
} Server;


/*############### Function Declarations ##############*/

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

// Construct endpointlist
EndpointList constructEndpointList(size_t size);
// Add endpoint and or method to list
bool registerEndpoint(EndpointList *list, char *url, HttpMethods httpMethod);
// Checks if endpoint exists and if the method is supported
bool supportsEndpoint(EndpointList *list, char *url, HttpMethods httpMethod); 
// Remove an endpoint
void freeEndpoint(EndpointList *list,char *url, HttpMethods httpMethod);
// Print endpoint list
void printEndpointList(EndpointList *list);

// Server construction
Server constructServer(int domain, int service, int protocol,
                       u_long interface, int port, int backlog,
                       EndpointList *endpoints, void (*launch)(Server *server));

#endif
