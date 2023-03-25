#ifndef CLIENT_H
#define CLIENT_H

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/http.h"

// Struct holding information needed to construct and use a client
typedef struct client {
    int domain;
    int service;
    int protocol;
    char *serverIpAddress;
    int port;
    int backlog;
    struct sockaddr_in address;
    int socket;
    bool (*launch)(struct client *client);
    HttpResponse *(*sendRequest)(struct client *client, HttpRequest *request);
    void (*stop)(struct client *client);
} Client;

// Client construction
Client *constructHttpClient(char *serverIpAddress, int port);

#endif
