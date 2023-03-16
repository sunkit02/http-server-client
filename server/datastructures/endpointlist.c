#include "../server.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static bool endpointSupportsHttpMethod(HttpEndPoint *endpoint, HttpMethods method) {
    bool support;
    switch (method) {
        case GET:
            support  = endpoint->canGet ? true : false;
            break;
        case POST:
            support  = endpoint->canPost ? true : false;
            break;
        case PUT:
            support  = endpoint->canPut ? true : false;
            break;
        case DELETE:
            support  = endpoint->canDelete ? true : false;
            break;
        default:
            support = false;
    }
    return support;
}

static bool resizeEndpointList(EndpointList *list) {
    // Double capacity
    list->capacity *= 2;
    HttpEndPoint *temp = realloc(list->endpoints, list->capacity);
    // Ensure the reallocation was successful
    if (temp == NULL) return false;

    list->endpoints = temp;
    return true;
}

static void addMethodSupportToEndpoint(HttpEndPoint *endpoint, HttpMethods method) {
    printf("Adding support for method=%d to endpoint='%s'\n", method, endpoint->url);
    switch (method) {
        case GET:
            endpoint->canGet = true;
            break;
        case POST:
            endpoint->canPost = true;
            break;
        case PUT:
            endpoint->canPut = true;
            break;
        case DELETE:
            endpoint->canDelete = true;
            break;
        case INVALID:
            return;
    }
}

// Construct endpointlist
EndpointList constructEndpointList(size_t capacity) {
    EndpointList endpointList;
    HttpEndPoint *endpoints = malloc(capacity * sizeof(HttpEndPoint));
    endpointList.endpoints = endpoints;
    endpointList.size = 0;
    endpointList.capacity = capacity;

    return endpointList;
}


// Add endpoint and or method to list
// Returns true if the endpoint already supports the httpMethod
// Returns false if failed to add method support to endpoint
bool registerEndpoint(EndpointList *list, char *url, HttpMethods httpMethod) {
    if (httpMethod == INVALID) return false;

    printf("Registering endpoint for url='%s', method=%d\n", url, httpMethod);

    HttpEndPoint *endpoints = list->endpoints;

    bool containsUrl = false;
    for (size_t i = 0; i < list->size; i++) {
        // Check if endpoint has been registered
        if (strcmp(endpoints[i].url, url) == 0) {
            addMethodSupportToEndpoint(&endpoints[i], httpMethod);
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
    // Copy url to another memory location in case original pointer gets freed
    // TODO: potentially need to add '\0' to end of new string
    char *tempUrl = malloc(strlen(url));
    memcpy(tempUrl, url, strlen(url));
    endpoint->url = tempUrl;
    addMethodSupportToEndpoint(endpoint, httpMethod);
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
        if (endpoints[i].canGet) {
            methodsStr[j++] = 'G';
            methodsStr[j++] = ',';
        }
        if (endpoints[i].canPost) {
            methodsStr[j++] = 'P';
            methodsStr[j++] = ',';
        }
        if (endpoints[i].canPut) {
            methodsStr[j++] = 'U';
            methodsStr[j++] = ',';
        }
        if (endpoints[i].canDelete) {
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
