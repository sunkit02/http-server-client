#include <string.h>
#include <stdlib.h>

#include "../server.h"
#include "../../utils/parsers.h"

void printRequest(HttpRequest *request) {
    char *methodStr;
    switch (request->method) {
        case GET:
            methodStr = "GET";
            break;
        case POST:
            methodStr = "POST";
            break;
        case PUT:
            methodStr = "PUT";
            break;
        case DELETE:
            methodStr = "DELETE";
            break;
        case INVALID:
            methodStr = "INVALID";
            break;
        default:
            methodStr = "UNKNOWN";
    }
    printf("Request(method=%s, url=%s, body=%s)\n",
           methodStr,
           request->url,
           request->body);
}

int main(void) {
    char *request = "sadfsadf / HTTP/1.1\r\nContent-Type: application/json\r\n\r\n{\"name\":\"Sun Kit\"}";
    printf("%s\n", request);
    HttpRequest requestObj = parseRequest(request);
    printRequest(&requestObj);
}
