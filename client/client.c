#include <arpa/inet.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "client.h"
#include "../utils/parsers.h"


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

static HttpResponse *sendRequest(Client *client, HttpRequest *request) {
    HttpResponse *response = malloc(sizeof(HttpResponse));
    char *requestString = stringifyHttpRequest(request);

    char responseBuffer[2048];
    
    send(client->socket, requestString, strlen(requestString), 0);
    recv(client->socket, &responseBuffer, sizeof(responseBuffer), 0);

    printf("Response received:\n%s\n", responseBuffer);

    response = parseHttpResponse(responseBuffer);

    return response;
}

static void stop(Client *client) {
    puts("Closing client socket...");
    shutdown(client->socket, SHUT_RDWR);
    close(client->socket);
}


// Construct an HTTP client capable of both sending requests and
// receiving responses to an HTTP server
Client *constructClient(int domain, int service, int protocol,
                       char *serverIpAddress, int port) {
    Client *client = malloc(sizeof(Client));

    client->domain = domain;
    client->service = service;
    client->protocol = protocol;
    client->serverIpAddress = serverIpAddress;
    client->port = port;

    client->address.sin_family = domain;
    client->address.sin_port = htons(port);
    client->address.sin_addr.s_addr = inet_addr(serverIpAddress);

    client->socket = socket(domain, service, protocol);
    if (client->socket == 0) {
        puts("Failed to create socket...");
        return NULL;
    }

    client->launch = launch;
    client->sendRequest = sendRequest;
    client->stop = stop;

    return client;
}