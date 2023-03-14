#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>

#include "/home/sunkit/src/c/simple_web_server/server.h"
#include "/home/sunkit/src/c/simple_web_server/parsers.h"


#define LISTENING_PORT 9001

// Objects created for handleKeyBoardInterrupt() to access
// in event of user keyboard interuptions
static Server server;
static int clientSocket;


void handleKeyBoardInterrupt(int sig) {
    puts("\nGracefully shutting down server...");

    printf("Closing socket on port %d...\n", LISTENING_PORT);
    shutdown(server.socket, SHUT_RDWR);
    close(server.socket);

    puts("Closing client socket...");
    shutdown(clientSocket, SHUT_RDWR);
    close(clientSocket);

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

void handleGetHome(int clientSocket) {
    char *homePage = "<h1>Welcome to the Home Page!</h1>";
    char *header = "HTTP/1.1 200 OK\r\n\n";
    char *response = calloc(1000, sizeof(char));
    strcat(response, header);
    strcat(response, homePage);
    send(clientSocket, response, strlen(response), 0);
    free(response);
}

void handleGetData(int clientSocket) {
    char *response = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n"
        "{\"name\": \"Sun Kit\"}";
    send(clientSocket, response, strlen(response), 0);
}

void handlePostUser(int clientSocket, HttpRequest request) {
    char *response = "HTTP/1.1 201 Created\r\n";
     


}

// Parses url and invokes the callback for the endpoint
// returns true for valid endpoint and false for invalid
bool handleRequest(HttpRequest *request, int clientSocket) {
    // identify endpoint by url
    if (strcmp(request->url, "/") == 0) {
        switch (request->method) {
            case GET:
                handleGetHome(clientSocket);
                break;
            default: 
                handleMethodNotSupported(clientSocket);
        }
    } else if (strcmp(request->url, "/data") == 0) {
        switch(request->method) {
            case GET:
                handleGetData(clientSocket);
                break;
            default:
                handleMethodNotSupported(clientSocket);
        }
    } else {
        // handle case when url is not a registered endpoint
        handleNotFound(clientSocket);
        return false;
    }
    return true;
}


// Function called to start server
// i.e. bind to socket, listen to socket, and handle incoming traffic
void launchServer(Server *server) {
    // Bind to socket
    bool bindSuccess = bind(server->socket, (struct sockaddr *) &server->address, sizeof(server->address));
    if (!bindSuccess) {
        printf("Failed to bind socket to port: %d", server->port);
        exit(EXIT_FAILURE);
    }

    // Listen to port 
    listen(server->socket, server->backlog); 
    printf("Listening on port %d\n", server->port);

    // Buffer for storing invoming requests
    char requestBuffer[2028];
    // Socket for sending responses back to client
    int clientSocket;

    // Continue to listen and handle requests until server stops
    while (true) {
        // Accept request
        clientSocket = accept(server->socket, NULL, NULL);
        puts("Handling a request...");

        // Read request
        recv(clientSocket, &requestBuffer, sizeof(requestBuffer), 0);

        // Parse request (printf for now )
        printf("\n\nRequest received:\n %s\n\n", requestBuffer);
        HttpRequest request = parseRequest(requestBuffer);
        printf("\nRequest(method=%d, url=%s, body=%s)\n", request.method, request.url, request.body);

        // Handle the request
        handleRequest(&request, clientSocket); 

        // Close clientSocket
        shutdown(clientSocket, SHUT_RDWR);
        close(clientSocket);
    }
}


int main() {
    // Register function for handling keyboard interuptions via C-c
    signal(SIGINT, handleKeyBoardInterrupt);

    // Register endpoints for server
    EndpointList endpointList = constructEndpointList(10);
    registerEndpoint(&endpointList, "/", GET);
    registerEndpoint(&endpointList, "/data", GET);
    registerEndpoint(&endpointList, "/user", POST);
    puts("Finished registering endpoints");
    printEndpointList(&endpointList);

    // Create server object
    server = constructServer(AF_INET, SOCK_STREAM,
                                    0, INADDR_ANY, LISTENING_PORT, 10,
                                    &endpointList,launchServer);
    // Start server
    server.launch(&server);
}


