#include "client.h"
#include <stdio.h>
#include <stdlib.h>

#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(void) {
    Client *client = constructClient(AF_INET, SOCK_STREAM, 0,
                                    "127.0.0.1", 9001);
    if (client == NULL) {
        puts("Failed to construct client");
        return 1;
    }

    if (client->launch(client) == false) {
        puts("Failed to launch server...");
        return 1;
    }

    HttpRequest request;
    request.url = "/";
    request.headers = NULL;
    request.method = GET;
    request.body = NULL;
    request.numOfHeaders = 0;

    HttpResponse *response = client->sendRequest(client, &request);
    printf("HttpResponse(status=%d, headers=%s, body=%s)\n",
                response->status, (char *)NULL, response->body);

    request.url = "/data";
    response = client->sendRequest(client, &request);
    printf("HttpResponse(status=%d, headers=%s, body=%s)\n",
                response->status, (char *)NULL, response->body);
}
