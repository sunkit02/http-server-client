#include "http.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void mapStatusCodeToStatusString(int statusCode, char *statusString);


HttpRequest *constructHttpRequest(HttpMethods method, char *url,
                                  HttpHeaderList *headerList, char *body) {
    HttpRequest *request = malloc(sizeof(HttpRequest));
    request->method = method;
    if (url != NULL)  {
        request->url = malloc(strlen(url) + 1);

        if (request->url != NULL) {
            strcpy(request->url, url);
        } else {
            httpRequestDestroy(request);
        
            return NULL;
        }
    }

    if (body != NULL)  {
        request->body = malloc(strlen(body) + 1);

        if (request->body != NULL) {
            strcpy(request->body, body);
        } else {
            httpRequestDestroy(request);

            return NULL;
        }
    }

    if (headerList != NULL) {
        request->headerList = headerList;
    }

    return request;
}

HttpResponse *constructHttpResponse(int statusCode,
                                    HttpHeaderList *headerList,
                                    char *body) {
    HttpResponse *response = malloc(sizeof(HttpResponse));
    response->statusCode = statusCode;

    response->status = calloc(100, sizeof(char));
    if (response->status == NULL) {
        perror("Failed to allocated memory for status");
        httpResponseDestroy(response);

        return NULL;
    }
    mapStatusCodeToStatusString(statusCode, response->status);
    if (strcmp(response->status, "Unknown") == 0) {
        httpResponseDestroy(response);

        return NULL;
    }

    if (body != NULL)  {
        response->body = malloc(strlen(body) + 1);

        if (response->body != NULL) {
            strcpy(response->body, body);
        } else {
            httpResponseDestroy(response);

            return NULL;
        }
    }

    response->headerList = headerList;

    return response;
}


void mapStatusCodeToStatusString(int statusCode, char *statusString) {
    switch (statusCode) {
        case 200:
            strcpy(statusString, "OK");
            break;
        case 201:
            strcpy(statusString, "Created");
            break;
        case 202:
            strcpy(statusString, "Accepted");
            break;
        case 203:
            strcpy(statusString, "Non-Authoritative Information");
            break;
        case 204:
            strcpy(statusString, "No Content");
            break;
        case 205:
            strcpy(statusString, "Reset Content");
            break;
        case 400:
            strcpy(statusString, "Bad Request");
            break;
        case 401:
            strcpy(statusString, "Unauthorized");
            break;
        case 403:
            strcpy(statusString, "Forbidden");
            break;
        case 404:
            strcpy(statusString, "Not Found");
            break;
        case 405:
            strcpy(statusString, "Method Not Allowed");
            break;
        case 406:
            strcpy(statusString, "Not Acceptable");
            break;
        case 411:
            strcpy(statusString, "Length Required");
            break;
        case 414:
            strcpy(statusString, "URI Too Long");
            break;
        case 500:
            strcpy(statusString, "Internal Server Error");
            break;
        default:
            strcpy(statusString, "Unknown");
            break;
    }
}
