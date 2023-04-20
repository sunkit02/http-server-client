#include "client.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

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
    puts("Constructed Client");

    bool keepRunning = true;
    char input = 0;

    HttpRequest *request;
    HttpResponse *response;
    while (keepRunning) {
        printf("\nSend request [y/r/n]? ");
        input = getchar();
        getchar();

        if (input == 'y') {
            request = constructHttpRequest(GET, "/", NULL, NULL);
            response = client->sendRequest(client, request);

            printf("Response: %s\n\n", response->body);
            httpRequestDestroy(request);
            httpResponseDestroy(response);
        } else if (input == 'r') {
            request = constructHttpRequest(GET, "/reset", NULL, NULL);
            response = client->sendRequest(client, request);

            printf("Response: %s\n\n", response->body);
            httpRequestDestroy(request);
            httpResponseDestroy(response);
        } else {
            puts("\nExiting...");
            break;
        }

    }

    destroyHttpClient(client);
}
