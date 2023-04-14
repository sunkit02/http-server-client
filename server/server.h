#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../lib/http.h"


typedef struct {
    HttpEndPoint *endpoints;
    size_t size;
    size_t capacity;
} EndpointList;


typedef struct server {
    int port;
    int backlog;
    struct sockaddr_in address;
    int socket;
    EndpointList *endpointList;
    void (*launch)(struct server *server);
} Server;


/*############### Function Declarations ##############*/

// Construct endpointlist
EndpointList *constructEndpointList(size_t size);
// Add endpoint and or method to list
bool registerEndpoint(EndpointList *list, char *url, HttpMethods httpMethod,
                      void (*callback)(int clientSocket, HttpRequest *request));
// Checks if endpoint exists and if the method is supported
bool supportsEndpoint(EndpointList *list, char *url, HttpMethods httpMethod); 
// Remove an endpoint
void freeEndpoint(EndpointList *list,char *url, HttpMethods httpMethod);
// Print endpoint list
void printEndpointList(EndpointList *list);

// Server construction
Server constructServer(int port, int backlog, EndpointList *endpointList, 
                       void (*launch)(Server *server));

#endif
