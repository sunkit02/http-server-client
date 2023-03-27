#include <signal.h>
#include <sys/socket.h>

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

void handleGetHome(int clientSocket) {
	char *response = "HTTP/1.1 200 OK\r\n\r\n"
		"<h1>Welcome to the Home Page!</h1>";
	send(clientSocket, response, strlen(response), 0);
}

void handleNotFound(int clientSocket) {
	char *response = "HTTP/1.1 404 Not Found\r\n\r\n<h1>Page not found</h1>";
	send(clientSocket, response, strlen(response), 0);
}

// Parses url and invokes the callback for the endpoint
// returns true for valid endpoint and false for invalid
bool handleRequest(HttpRequest *request, int clientSocket) {
	// identify endpoint
	if (strcmp(request->url, "/") == 0 &&
        request->method == GET) handleGetHome(clientSocket);
	else handleNotFound(clientSocket);
	return true;
}

void launchServer(Server *server) {
	// start server
	bool bindSuccess= bind(server->socket, (struct sockaddr *) &server->address, sizeof(server->address));
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
		puts("Handling a request...");

		// Read request
		recv(clientSocket, &requestBuffer, sizeof(requestBuffer), 0);

		// Parse request (printf for now )
		printf("Request received:\n %s\n", requestBuffer);
		HttpRequest *request = parseHttpRequest(requestBuffer);
		printf("Request(method=%d, url=%s, body=%s)\n", request->method, request->url, request->body);

		// Invoke callback for request
		// handleGetHome(clientSocket);
		handleRequest(request, clientSocket); 

		// Close clientSocket
		shutdown(clientSocket, SHUT_RDWR);
		close(clientSocket);
	}
}

int main(void) {
    // Register callback for handling keyboard interuptions via <C-c>
    signal(SIGINT, handleKeyBoardInterrupt);

	EndpointList endpointList = constructEndpointList(10);
	registerEndpoint(&endpointList, "/", GET);

	server = constructServer(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 9001, 10, &endpointList,launchServer);
	server.launch(&server);
}
