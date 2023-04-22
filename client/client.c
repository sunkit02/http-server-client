#include <arpa/inet.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "client.h"
#include "../lib/parsers.h"


///////////////////////////// DEPRECATED /////////////////////////////
// Starts running the client by connecting to the server through 
// the socket passed in by the Client struct.
// Returns true if the connection was successfully established
// and returns false if failed to connect to server
static bool launch(Client *client) {
    int connectionFailed = connect(client->socket,
                                   (struct sockaddr *) &client->address,
                                   sizeof(client->address));
    
    if (connectionFailed) {
        puts("Failed to connect to socket...");
        return false;
    }
    puts("Connected to socket");

    return true;
}


static bool connectToServer(Client *client) {
    // Create socket
    client->socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client->socket == -1) {
        puts("Failed to create socket...");
        return false;
    }

    // Establish connection with server
    int connectionFailed = connect(client->socket,
                                   (struct sockaddr *) &client->address,
                                   sizeof(client->address));
    
    if (connectionFailed) {
        puts("Failed to connect to socket...");
        return false;
    }

    return true;
}

// Sends an HTTP request synchronously and returns an HttpResponse
// struct that contains the HTTP response from the server
static HttpResponse *sendRequest(Client *client, HttpRequest *request) {
    if (!connectToServer(client)) return NULL;

    char *requestString = stringifyHttpRequest(request);

    char responseBuffer[2048];
    
    send(client->socket, requestString, strlen(requestString) + 1, 0);
    recv(client->socket, &responseBuffer, sizeof(responseBuffer), 0);

    // Close socket after each request
    shutdown(client->socket, SHUT_RDWR);
    close(client->socket);
    // Free requestString
    free(requestString);

    HttpResponse *response = parseHttpResponse(responseBuffer);
    if (response == NULL) return NULL;

    return response;
}

///////////////////////////// DEPRECATED /////////////////////////////
// Stops the client from running by closing down the socket
static void stop(Client *client) {
    puts("Closing client socket...");
    shutdown(client->socket, SHUT_RDWR);
    close(client->socket);
}


// Construct an HTTP client capable of both sending requests and
// receiving responses to an HTTP server
Client *constructHttpClient(char *serverIpAddress, int port) {
    Client *client = malloc(sizeof(Client));

    client->domain = AF_INET; // WARN: DEPRECATED
    client->service = SOCK_STREAM; // WARN: DEPRECATED
    client->protocol = 0; // WARN: DEPRECATED
    //
    client->serverIpAddress = malloc(strlen(serverIpAddress) + 1);
    if (serverIpAddress == NULL) return NULL;
    strcpy(client->serverIpAddress, serverIpAddress);

    client->port = port;

    client->address.sin_family = AF_INET;
    client->address.sin_port = htons(port);
    client->address.sin_addr.s_addr = inet_addr(serverIpAddress);

    client->sendRequest = sendRequest;

    // DEPRECATED
    client->launch = launch;
    // DEPRECATED
    client->stop = stop;

    return client;
}

void destroyHttpClient(Client *client) {
    if (client != NULL) {
        free(client->serverIpAddress);
    }
    free(client);
}
