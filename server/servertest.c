#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>

#include "server.h"
#include "../lib/parsers.h"


#define LISTENING_PORT 9001
#define BACK_LOG 10

// Objects created for handleKeyBoardInterrupt() to access
// in event of user keyboard interuptions
static HttpServer *server;
static int clientSocket;


void handleKeyBoardInterrupt(int sig) {
    puts("\nGracefully shutting down server...");

    printf("Closing socket on port %d...\n", LISTENING_PORT);
    shutdown(server->socket, SHUT_RDWR);
    close(server->socket);

    puts("Closing client socket...");
    shutdown(clientSocket, SHUT_RDWR);
    close(clientSocket);

    destroyHttpServer(server);

    puts("Successfully shutdown server");
    exit(0);
}

void handleNotFound(int clientSocket) {
    char *response = "HTTP/1.1 404 Not Found\r\n\r\n"
                    "<h1>404 The page requested is not found.</h1>";
    send(clientSocket, response, strlen(response), 0);
}

void handleMethodNotSupported(int clientSocket) {
    char *response = "HTTP/1.1 405 Method Not Allowed\r\n\r\n"
                    "<h1>405 Http method used is not allowed</h1>";
    send(clientSocket, response, strlen(response), 0);
}

void handleGetHome(int clientSocket, HttpRequest *request) {
    char *homePage = "<h1>Welcome to the TEST Home Page!</h1>";
    char *header = "HTTP/1.1 200 OK\r\n\n";
    char *response = calloc(1000, sizeof(char));
    strcat(response, header);
    strcat(response, homePage);
    send(clientSocket, response, strlen(response), 0);
    free(response);
}

void handleGetData(int clientSocket, HttpRequest *request) {
    char *response = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n"
        "{\"name\": \"Sun Kit\"}";
    send(clientSocket, response, strlen(response), 0);
}

void handlePostUser(int clientSocket, HttpRequest *request) {
    char *response = "HTTP/1.1 201 Created\r\n";
    send(clientSocket, response, strlen(response), 0);
}

// Parses url and invokes the callback for the endpoint
// returns true for valid endpoint and false for invalid
void handleHttpRequest(HttpServer *server, int clientSocket, HttpRequest *request) {
    EndpointList *endpointList = server->endpointList;
    HttpEndPoint **endpoints = endpointList->endpoints;

    for (size_t i = 0; i < endpointList->size; i++) {
        // Check if the endpoint exists for the request
        // 1. Url exists
        if (strcmp(request->url, endpoints[i]->url) == 0) {
            // 2. HTTP method is supported
            if (endpoints[i]->callbacks[request->method] != NULL) {
                // Invoke callback if endpoints exits
                endpoints[i]->callbacks[request->method](clientSocket, request);
            } else {
                handleMethodNotSupported(clientSocket);
            }
            // Return anyway if URL exists
            return;
        }
    }
    // Catch all for unmatched URLs
    handleNotFound(clientSocket);
}


// Function called to start server
// i.e. bind to socket, listen to socket, and handle incoming traffic
void launchServer(HttpServer *server) {
    // Bind to socket
    int bindResult = bind(server->socket, (struct sockaddr *) &server->address, sizeof(server->address));
    if (bindResult == -1) {
        printf("\n\nFailed to bind socket to port: %d\n", server->port);
        puts("Aborting...");

        destroyHttpServer(server);
        exit(EXIT_FAILURE);
    }

    // Listen to port 
    int listenResult = listen(server->socket, server->backlog); 
    if (listenResult == -1) {
        printf("\n\nFailed to listent to port: %d\n", server->port);
        puts("Aborting...");

        destroyHttpServer(server);
        exit(EXIT_FAILURE);
    }
    printf("Listening on port %d\n", server->port);

    // Buffer for storing invoming requests
    char requestBuffer[2028];
    // Socket for sending responses back to client
    int clientSocket;

    // Listen to hand handle n number of requests
    int i = 0;
    int n = 3;
    while (i < n) {
        // Accept request
        clientSocket = accept(server->socket, NULL, NULL);
        puts("Handling a request...");

        // Read request
        recv(clientSocket, &requestBuffer, sizeof(requestBuffer), 0);

        // Parse request (printf for now )
        printf("\n\nRequest received:\n %s\n\n", requestBuffer);
        HttpRequest *request = parseHttpRequest(requestBuffer);
        printf("\nRequest(method=%d, url=%s, body=%s)\n", request->method, request->url, request->body);

        // Handle the request
        handleHttpRequest(server, clientSocket, request); 
        // Free memory used by request
        httpRequestDestroy(request);

        // Close clientSocket
        shutdown(clientSocket, SHUT_RDWR);
        close(clientSocket);

        i++;
    }
}


int main() {
    // Register function for handling keyboard interuptions via C-c
    signal(SIGINT, handleKeyBoardInterrupt);

    // Register endpoints for server
    EndpointList *endpointList = constructEndpointList(10);
    registerEndpoint(endpointList, "/", GET, handleGetHome);
    registerEndpoint(endpointList, "/data", GET, handleGetData);
    registerEndpoint(endpointList, "/user", POST, handlePostUser);

    printEndpointList(endpointList);

    // Create server object
    server = constructHttpServer(LISTENING_PORT, BACK_LOG, endpointList, launchServer);

    // Start server
    server->launch(server);
    destroyHttpServer(server);
}
