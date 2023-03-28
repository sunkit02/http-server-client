#include "../server.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define NUM_OF_SUPPORTED_METHODS 4

// static bool endpointSupportsHttpMethod(HttpEndPoint *endpoint, HttpMethods method) {
//     bool support;
//     switch (method) {
//         case GET:
//             support  = endpoint->canGet ? true : false;
//             break;
//         case POST:
//             support  = endpoint->canPost ? true : false;
//             break;
//         case PUT:
//             support  = endpoint->canPut ? true : false;
//             break;
//         case DELETE:
//             support  = endpoint->canDelete ? true : false;
//             break;
//         default:
//             support = false;
//     }
//     return support;
// }

static bool resizeEndpointList(EndpointList *list) {
    // Double capacity
    list->capacity *= 2;
    HttpEndPoint *temp = realloc(list->endpoints, list->capacity);
    // Ensure the reallocation was successful
    if (temp == NULL) return false;

    list->endpoints = temp;
    return true;
}

static void addMethodSupportToEndpoint(HttpEndPoint *endpoint,
                                       HttpMethods method,
                                       void (*callback)(int clientSocket, HttpRequest *request)) {
    printf("Adding support for method=%d to endpoint='%s'\n", method, endpoint->url);

    // Add method and callback to endpoint
    endpoint->callbacks[method] = callback;
}

// Construct endpointlist
EndpointList *constructEndpointList(size_t capacity) {
    EndpointList *endpointList = malloc(sizeof(EndpointList));

    endpointList->endpoints = calloc(capacity, sizeof(HttpEndPoint));
    if (endpointList->endpoints == NULL) return NULL;

    endpointList->size = 0;
    endpointList->capacity = capacity;

    return endpointList;
}


// Add endpoint and or method to list
// Returns true if the endpoint already supports the httpMethod
// Returns false if failed to add method support to endpoint
bool registerEndpoint(EndpointList *list, 
                      char *url,
                      HttpMethods httpMethod,
                      void (*callback)(int clientSocket, HttpRequest *request)) {
    if (httpMethod == INVALID || callback == NULL || url == NULL) return false;

    printf("Registering endpoint for url='%s', method=%d\n", url, httpMethod);

    HttpEndPoint *endpoints = list->endpoints;

    bool containsUrl = false;
    for (size_t i = 0; i < list->size; i++) {
        // Check if endpoint has been registered
        if (strcmp(endpoints[i].url, url) == 0) {
            addMethodSupportToEndpoint(&endpoints[i], httpMethod, callback);
            return true;
        }
    }
    // Add url endpoint to list
    if (list->size == list->capacity) {
        bool resizeSuccess = resizeEndpointList(list);
        // Indicate failure to add method support if failed to resize
        if (!resizeSuccess) return false;
    }
    // Create endpoint 
    HttpEndPoint *endpoint = malloc(sizeof(HttpEndPoint));

    // Explicitly set all callbacks to NULL
    for (size_t i = 0; i < NUM_OF_SUPPORTED_METHODS; i++) {
        endpoint->callbacks[i] = NULL;
    }

    // Copy url to another memory location in case original pointer gets freed
    char *tempUrl = malloc(strlen(url) + 1);
    strcpy(tempUrl, url);
    endpoint->url = tempUrl;

    addMethodSupportToEndpoint(endpoint, httpMethod, callback);

    list->endpoints[list->size] = *endpoint;
    list->size++;
    return true;
}

// Checks if endpoint exists and if the method is supported
bool supportsEndpoint(EndpointList *list, char *url, HttpMethods httpMethod);

// Remove an endpoint
void freeEndpoint(EndpointList *list,char *url, HttpMethods httpMethod);

// Print endpoint list
void printEndpointList(EndpointList *list) {
    HttpEndPoint *endpoints = list->endpoints;
    printf("[");
    for (size_t i = 0; i < list->size; i++) {
        char methodsStr[10];
        size_t j = 0;
        methodsStr[j++] = '[';
        if (endpoints[i].callbacks[GET] != NULL) {
            methodsStr[j++] = 'G';
            methodsStr[j++] = ',';
        }
        if (endpoints[i].callbacks[POST] != NULL) {
            methodsStr[j++] = 'P';
            methodsStr[j++] = ',';
        }
        if (endpoints[i].callbacks[PUT] != NULL) {
            methodsStr[j++] = 'U';
            methodsStr[j++] = ',';
        }
        if (endpoints[i].callbacks[DELETE] != NULL) {
            methodsStr[j++] = 'D';
            methodsStr[j++] = ',';
        }
        methodsStr[--j] = ']';
        methodsStr[++j] = '\0';

        printf("Endpoint(url=%s, methods=%s)%s", endpoints[i].url, methodsStr,
               i == (list->size - 1) ? "" : ", ");
    }
    puts("]");
}
