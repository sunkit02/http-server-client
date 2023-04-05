#include <stdio.h>
#include <sys/types.h>

#include "server.h"
#include "../utils/parsers.h"

Server constructServer(int port, int backlog, EndpointList *endpointList, 
                       void (*launch)(Server *server)) {
    Server server;

    server.port = port;
    server.backlog = backlog;

    server.address.sin_family = AF_INET;
    server.address.sin_port = htons(port);
    server.address.sin_addr.s_addr = INADDR_ANY;


    server.socket = socket(AF_INET, SOCK_STREAM, 0);

    if (server.socket == 0) {
        perror("Failed to connect socket...\n");
        exit(1);
    }

    if (bind(server.socket,
             (struct sockaddr *)&server.address,
             sizeof(server.address)) < 0) {
        perror("Failed to bind socket...\n");
        exit(1);
    }

    if (listen(server.socket, server.backlog) < 0) {
        perror("Failed to start listening...");
        exit(1);
    }

    server.endpointList = endpointList;
    server.launch = launch;

    return server;
}

