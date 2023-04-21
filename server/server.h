#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../lib/http.h"
#include "../lib/parsers.h"

typedef struct {
    HttpEndPoint **endpoints;
    size_t size;
    size_t capacity;
} EndpointList;


typedef struct HttpServer {
    int port;
    int backlog;
    struct sockaddr_in address;
    int socket;
    EndpointList *endpointList;
    void (*launch)(struct HttpServer *server);
} HttpServer;


/*############### Function Declarations ##############*/

// Construct an endpointlist with capacity of the size parameter.
// NOTE: EnpointList struct returned by this functon needs to be freed 
// using the destroyServer function
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
// Frees all memory allocated by endpointList
void destroyEndpointList(EndpointList *list);

// Server construction
// NOTE: Server struct returned needs to be freed using the destroyServer function.
HttpServer *constructHttpServer(int port, int backlog, EndpointList *endpointList, 
                       void (*launch)(HttpServer *server));

// Responds to a request by serialzing the HttpResponse passed in and
// send through socket passsed in as clientSocket.
// NOTE: Does not consume the socket or HttpResponse struct passed in
void respondToHttpRequest(int clientSocket, HttpResponse *response);


// Frees all memory allocated by server
void destroyHttpServer(HttpServer *server);
#endif
