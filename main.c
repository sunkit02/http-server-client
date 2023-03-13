#include "server.h"
#include "parsers.h"
#include <sys/socket.h>

void handleGetHome(int clientSocket) {
	char *response = "HTTP/1.1 200 OK\r\n\n"
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
	if (strcmp(request->url, "/") == 0) handleGetHome(clientSocket);
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
	int clientSocket;
	while (true) {
		// accept request
		clientSocket = accept(server->socket, NULL, NULL);
		puts("Handling a request...");

		// read request
		recv(clientSocket, &requestBuffer, sizeof(requestBuffer), 0);

		// parse request (printf for now )
		printf("Request received:\n %s\n", requestBuffer);
		HttpRequest request = parseRequest(requestBuffer);
		printf("Request(method=%d, url=%s, body=%s)\n", request.method, request.url, request.body);

		// invoke callback for request
		// handleGetHome(clientSocket);
		handleRequest(&request, clientSocket); 

		// close clientSocket
		shutdown(clientSocket, SHUT_RDWR);
		close(clientSocket);
	}
}

int main(void) {
	EndpointList endpointList = constructEndpointList(10);
	registerEndpoint(&endpointList, "/", GET);

	Server server = constructServer(AF_INET, SOCK_STREAM,
								 0, INADDR_ANY, 9001, 10,
								 &endpointList,launchServer);
	server.launch(&server);
}
