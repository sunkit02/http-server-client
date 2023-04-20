#include <stdio.h>
#include <sys/types.h>

#include "server.h"
#include "../lib/parsers.h"

Server *constructServer(int port, int backlog, EndpointList *endpointList, 
                       void (*launch)(Server *server)) {
    Server *server = malloc(sizeof(Server));

    server->port = port;
    server->backlog = backlog;

    server->address.sin_family = AF_INET;
    server->address.sin_port = htons(port);
    server->address.sin_addr.s_addr = INADDR_ANY;


    server->socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server->socket == 0) {
        perror("Failed to connect socket...\n");
        exit(1);
    }

    server->endpointList = endpointList;
    server->launch = launch;

    return server;
}

void destroyServer(Server *server) {
    if (server->endpointList != NULL) {
        destroyEndpointList(server->endpointList);
    }
    free(server);
}
