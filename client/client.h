#ifndef CLIENT_H
#define CLIENT_H

#include "../lib/http.h"
#include "../lib/parsers.h"

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <arpa/inet.h>

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>


// Struct holding information needed to construct and use a client
typedef struct HttpClient {
    char *serverIpAddress;
    int port;
    int backlog;
    struct sockaddr_in address;
    int socket;
} HttpClient;

// Client construction
HttpClient *constructHttpClient(char *serverIpAddress, int port);

// Sends an HTTP request synchronously and returns an HttpResponse
// struct that contains the HTTP response from the server.
// @param client - pointer to the HttpClient struct sending the request.
// @param request - pointer to the HttpRequest struct to be sent.
// @return - HttpResponse struct from server if success and NULL if failed to send
// @note - HttpResponse returned needs to be freed with
HttpResponse *sendHttpRequest(HttpClient *client, HttpRequest *request);

void destroyHttpClient(HttpClient *client);

#endif
