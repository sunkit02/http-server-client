#include <stdio.h>
#include "server.h"

int n = 0;

// ##################### Function Prototypes ##################### //
void handleGetHome(int clientSocket, HttpRequest *request);
void handleReset(int clientSocket, HttpRequest *request);

// ##################### Register Endpoints ##################### //
void registerEndpointsForServer(EndpointList *endpointList) {
	registerEndpoint(endpointList, "/", GET, handleGetHome);
	registerEndpoint(endpointList, "/reset", GET, handleReset);
}


// ##################### Function Definitions ##################### //
void handleGetHome(int clientSocket, HttpRequest *request) {
    // char responseBody[1024] = "<h1>Welcome to the Home Page for the %d time!</h1>"; 
    char responseBody[1024] = { 0 };
    sprintf(responseBody, "<h1>Welcome to the Home Page for the %d time!</h1>", ++n);
    
    HttpResponse *response = constructHttpResponse(200, NULL, responseBody);

    respondToRequest(clientSocket, response);
    httpResponseDestroy(response);
}

void handleReset(int clientSocket, HttpRequest *request) {
    char *responseBody = "<h1>Successfully reset counter to 0</h1>";
    HttpResponse *response = constructHttpResponse(201, NULL, responseBody);

    n = 0;

    respondToRequest(clientSocket, response);
    httpResponseDestroy(response);
}
