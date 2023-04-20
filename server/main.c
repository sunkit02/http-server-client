#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "server.h"
#include "../lib/parsers.h"

#define LOG_INFO(format, ...) printf(format, ##__VA_ARGS__)
#define LOG_ERROR(format, ...) printf(format, ##__VA_ARGS__)


// Objects created for handleKeyBoardInterrupt() to access
// in event of user keyboard interuptions
static Server *server;
static int clientSocket;

// Function defined in game.c to register functions that are
// created in game.c.
void registerEndpointsForServer(EndpointList *list);

void respondToRequest(int clientSocket, HttpResponse *response) {
    char *responseStr = stringifyHttpResponse(response);
    if (responseStr == NULL) {
        LOG_ERROR("Failed to stringifyHttpResponse");
        return;
    }
    send(clientSocket, responseStr, strlen(responseStr), 0);
    free(responseStr);
}

void shutdownServerGracefully(int sig) {
    puts("\nGracefully shutting down server...");

    printf("Closing socket on port %d...\n", server->port);
    shutdown(server->socket, SHUT_RDWR);
    close(server->socket);

    puts("Closing client socket...");
    shutdown(clientSocket, SHUT_RDWR);
    close(clientSocket);
    
    puts("Successfully shutdown server");
    exit(0);
}

void handleSegFault(int sig) {
    puts("Segmentation fault!!!");
    shutdownServerGracefully(sig);
}

void handleSigTerm(int sig) {
    puts("SIGTERM!!!");
    shutdownServerGracefully(sig);
}

void handleNotFound(int clientSocket) {
    HttpResponse *response = 
        constructHttpResponse(404, NULL, "<h1>Page not found</h1>");
    respondToRequest(clientSocket, response);
    httpResponseDestroy(response);
}

void handleMethodNotSupported(int clientSocket) {
    // NOT supported
}

// Parses url and invokes the callback for the endpoint
// returns true for valid endpoint and false for invalid
void handleRequest(Server *server, int clientSocket, HttpRequest *request) {
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

void launchServer(Server *server) {
	// start server
	int bindSuccess= bind(server->socket, 
                           (struct sockaddr *) &server->address,
                           sizeof(server->address));

	if (bindSuccess == -1) {
		printf("Failed to bind socket to port: %d\n", server->port);
        destroyServer(server);
		exit(EXIT_FAILURE);
	}

    // listen to port 
    int listenResult = listen(server->socket, server->backlog); 
    if (listenResult == -1) {
        printf("\n\nFailed to listent to port: %d\n", server->port);
        puts("Aborting...");

        destroyServer(server);
        exit(EXIT_FAILURE);
    }

	printf("Listening on port %d\n", server->port);

    size_t bufferSize = 4096;
	char *requestBuffer = calloc(bufferSize, sizeof(char));
    
    // Main listening loop
	while (true) {
		// accept request
		clientSocket = accept(server->socket, NULL, NULL);
        puts("_______________________");
        puts("________REQUEST________");
		puts("Handling a request...");

		// Read request
		recv(clientSocket, requestBuffer, bufferSize, 0);

		// Parse request (printf for now )
		printf("---\n%s\n", requestBuffer);
        puts("---");
		HttpRequest *request = parseHttpRequest(requestBuffer);
		printf("Request(method=%d, url=%s, body=%s)\n",
                request->method, request->url, request->body);
        puts("__________END__________\n\n");

		// Invoke callback for request
		// handleGetHome(clientSocket);
		handleRequest(server, clientSocket, request); 

		// Close clientSocket
		shutdown(clientSocket, SHUT_RDWR);
		close(clientSocket);
        memset(requestBuffer, 0, bufferSize);
	}
    free(requestBuffer);
}

int main(void) {
    // Register callback for handling keyboard interuptions via <C-c>
    signal(SIGINT, shutdownServerGracefully);
    signal(SIGSEGV, handleSegFault);		
    signal(SIGTERM, handleSigTerm);

    EndpointList *endpointList = constructEndpointList(10);
    registerEndpointsForServer(endpointList);

    server = constructServer(9001, 10, endpointList, launchServer);
	server->launch(server);
    destroyServer(server);
}
