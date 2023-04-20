#include "client.h"
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

    // if (client->launch(client) == false) {
    //     puts("Failed to client server...");
    //     return 1;
    // }

    puts("launched client");


    HttpRequest request;
    request.url = "/";
    request.headerList = NULL;
    request.method = GET;
    request.body = NULL;

    printf("sizeof(HttpResponse)=%zu\n", sizeof(HttpResponse));

    puts("Sending request 1");
    HttpResponse *response = client->sendRequest(client, &request);
    if (response) {
        printf("\n\nHttpResponse(status=%d, headers=%s, body=%s)\n\n",
               response->statusCode, (char *)NULL, response->body);

        httpResponseDestroy(response);
    }


    puts("Sending request 2");
    request.url = "/data";
    response = client->sendRequest(client, &request);
    if (response) {
        printf("\n\nHttpResponse(status=%d, headers=%s, body=%s)\n\n",
               response->statusCode, (char *)NULL, response->body);

        httpResponseDestroy(response);
    }


    puts("Sending request 3");
    char newUrl[] = "/";
    request.url = newUrl;
    response = client->sendRequest(client, &request);
    if (response) {
        printf("\n\nHttpResponse(status=%d, headers=%s, body=%s)\n\n",
               response->statusCode, (char *)NULL, response->body);

        httpResponseDestroy(response);
    }
    free(client);
}
