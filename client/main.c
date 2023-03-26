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
    Client *client = constructHttpClient("127.0.0.1", 9001);
    if (client == NULL) {
        puts("Failed to construct client");
        return 1;
    }
    puts("Cosntructed Client");

    if (client->launch(client) == false) {
        puts("Failed to client server...");
        return 1;
    }

    puts("launched client");


    HttpRequest request;
    request.url = "/";
    request.headerList = NULL;
    request.method = GET;
    request.body = NULL;
    request.numOfHeaders = 0;

    puts("Sending request");
    HttpResponse *response = client->sendRequest(client, &request);
    printf("\n\nHttpResponse(status=%d, headers=%s, body=%s)\n\n",
                response->statusCode, (char *)NULL, response->body);
    puts("Request sent");

    puts("Sending request2");
    request.url = "/data";
    response = client->sendRequest(client, &request);
    printf("\n\nHttpResponse(status=%d, headers=%s, body=%s)\n\n",
                response->statusCode, (char *)NULL, response->body);


    char newUrl[] = "/";
    request.url = newUrl;
    response = client->sendRequest(client, &request);
    printf("\n\nHttpResponse(status=%d, headers=%s, body=%s)\n\n",
                response->statusCode, (char *)NULL, response->body);
}
