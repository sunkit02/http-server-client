#include "client.h"

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    HttpClient *client = constructHttpClient("127.0.0.1", 9001);
    if (client == NULL) {
        puts("Failed to construct client");
        return 1;
    }


    HttpRequest request;
    request.url = "/";
    request.headerList = NULL;
    request.method = GET;
    request.body = NULL;

    puts("\nSending request 1");
    HttpResponse *response = sendHttpRequest(client, &request);
    if (response) {
        printf("\nHttpResponse(status=%d, headers=%s, body=%s)\n\n",
               response->statusCode, (char *)NULL, response->body);

        httpResponseDestroy(response);
    }


    puts("\nSending request 2");
    request.url = "/data";
    response = sendHttpRequest(client, &request);
    if (response) {
        printf("\nHttpResponse(status=%d, headers=%s, body=%s)\n\n",
               response->statusCode, (char *)NULL, response->body);

        httpResponseDestroy(response);
    }


    puts("\nSending request 3");
    char newUrl[] = "/";
    request.url = newUrl;
    response = sendHttpRequest(client, &request);
    if (response) {
        printf("\nHttpResponse(status=%d, headers=%s, body=%s)\n\n",
               response->statusCode, (char *)NULL, response->body);

        httpResponseDestroy(response);
    }
    free(client);
}
