#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <unistd.h>

#include "server.h"
#include "urlparser.h"

Server constructServer(int domain, int service, int protocol,
                       u_long interface, int port, int backlog,
                       EndpointList *endpointList, void (*launch)(Server *server)) {
    Server server;

    server.domain = domain;
    server.service = service;
    server.protocol = protocol;
    server.interface = interface;
    server.port = port;
    server.backlog = backlog;

    server.address.sin_family = domain;
    server.address.sin_port = htons(port);
    server.address.sin_addr.s_addr = interface;

    server.socket = socket(domain, service, protocol);
    if (server.socket == 0) {
        perror("Failed to connect socket...\n");
        exit(1);
    }

    if (bind(server.socket, (struct sockaddr *)&server.address,
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

