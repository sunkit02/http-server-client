#include <signal.h>
#include <sys/socket.h>
#include <unistd.h>
#include "server.h"
#include "../utils/parsers.h"


// Objects created for handleKeyBoardInterrupt() to access
// in event of user keyboard interuptions
static Server server;
static int clientSocket;


void handleKeyBoardInterrupt(int sig) {
    puts("\nGracefully shutting down server...");

    printf("Closing socket on port %d...\n", server.port);
    shutdown(server.socket, SHUT_RDWR);
    close(server.socket);

    puts("Closing client socket...");
    shutdown(clientSocket, SHUT_RDWR);
    close(clientSocket);

    puts("Successfully shutdown server");
    exit(0);
}

void handleGetHome(int clientSocket, HttpRequest *request) {
	char *response = "HTTP/1.1 200 OK\r\n\r\n"
		"<h1>Welcome to the Home Page!</h1>";
	send(clientSocket, response, strlen(response), 0);
}

void handleNotFound(int clientSocket) {
	char *response = "HTTP/1.1 404 Not Found\r\n\r\n<h1>Page not found</h1>";
	send(clientSocket, response, strlen(response), 0);
}

void handleMethodNotSupported(int clientSocket) {
    // NOT supported
}

// Parses url and invokes the callback for the endpoint
// returns true for valid endpoint and false for invalid
void handleRequest(Server *server, int clientSocket, HttpRequest *request) {
    EndpointList *endpointList = server->endpointList;
    HttpEndPoint *endpoints = endpointList->endpoints;

    for (size_t i = 0; i < endpointList->size; i++) {
        // Check if the endpoint exists for the request
        // 1. Url exists
        if (strcmp(request->url, endpoints[i].url) == 0) {
            // 2. HTTP method is supported
            if (endpoints[i].callbacks[request->method] != NULL) {
                // Invoke callback if endpoints exits
                endpoints[i].callbacks[request->method](clientSocket, request);
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
	bool bindSuccess= bind(server->socket, 
                           (struct sockaddr *) &server->address,
                           sizeof(server->address));
	if (!bindSuccess) {
		printf("Failed to bind socket to port: %d", server->port);
		exit(EXIT_FAILURE);
	}
    // listen to port 
    listen(server->socket, server->backlog); 
	printf("Listening on port %d\n", server->port);

	char requestBuffer[2028];
    
    // Main listening loop
	while (true) {
		// accept request
		clientSocket = accept(server->socket, NULL, NULL);
        puts("_______________________");
        puts("________REQUEST________");
		puts("Handling a request...");

		// Read request
		recv(clientSocket, &requestBuffer,
             sizeof(requestBuffer), 0);

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
	}
}

int main(void) {
    // Register callback for handling keyboard interuptions via <C-c>
    signal(SIGINT, handleKeyBoardInterrupt);

	EndpointList *endpointList = constructEndpointList(10);
	registerEndpoint(endpointList, "/", GET, handleGetHome);

    server = constructServer(9001, 10, endpointList, launchServer);
	server.launch(&server);
}
