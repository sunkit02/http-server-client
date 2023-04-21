#include <arpa/inet.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "client.h"
#include "../lib/parsers.h"


static bool connectToServer(HttpClient *client) {
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

// TODO: make it take in address to which to send to and not an HttpClient
HttpResponse *sendHttpRequest(HttpClient *client, HttpRequest *request) {
    if (!connectToServer(client)) return NULL;

    char *requestString = stringifyHttpRequest(request);

    char responseBuffer[2048];
    
    // Send request to server and read response 
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


// Construct an HTTP client capable of both sending requests and
// receiving responses to an HTTP server
HttpClient *constructHttpClient(char *serverIpAddress, int port) {
    HttpClient *client = calloc(1, sizeof(HttpClient));

    // Create copy for serverIpAddress
    client->serverIpAddress = malloc(strlen(serverIpAddress) + 1);
    if (serverIpAddress == NULL) return NULL;
    strcpy(client->serverIpAddress, serverIpAddress);

    client->port = port;

    client->address.sin_family = AF_INET;
    client->address.sin_port = htons(port);
    client->address.sin_addr.s_addr = inet_addr(serverIpAddress);

    return client;
}

void destroyHttpClient(HttpClient *client) {
    if (client != NULL) {
        free(client->serverIpAddress);
    }
    free(client);
}
