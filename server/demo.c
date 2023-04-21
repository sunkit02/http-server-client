#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "server.h"
#include "../lib/game.h"
#include "../lib/base64.h"


static int numVisits = 0;
static int numPlayers = 0;
static Player players[4];


// ##################### Function Prototypes ##################### //
void handleGetHome(int clientSocket, HttpRequest *request);
void handleReset(int clientSocket, HttpRequest *request);
void handlePlayer(int clientSocket, HttpRequest *request);
void handleAddPlayer(int clientSocket, HttpRequest *request);
void handleGetPlayers(int clientSocket, HttpRequest *request);


// ##################### Register Endpoints ##################### //
void registerEndpointsForServer(EndpointList *endpoints) {

	registerEndpoint(endpoints, "/", GET, handleGetHome);
	registerEndpoint(endpoints, "/reset", GET, handleReset);
	registerEndpoint(endpoints, "/player", POST, handlePlayer);
    registerEndpoint(endpoints, "/players/add", POST, handleAddPlayer);
    registerEndpoint(endpoints, "/players/get", GET, handleGetPlayers);
}


// ##################### Function Definitions ##################### //
void handleGetHome(int clientSocket, HttpRequest *request) {

    char responseBody[1024];

    sprintf(responseBody, 
            "<h1>Welcome to the Home Page for the %d time!</h1>", ++numVisits);
    
    HttpResponse *response = constructHttpResponse(200, NULL, responseBody);

    respondToHttpRequest(clientSocket, response);

    httpResponseDestroy(response);
}


void handleReset(int clientSocket, HttpRequest *request) {

    numVisits = 0;

    static char *responseBody = "<h1>Successfully reset counter to 0</h1>";

    HttpResponse *response = constructHttpResponse(201, NULL, responseBody);

    respondToHttpRequest(clientSocket, response);

    httpResponseDestroy(response);
}


void handlePlayer(int clientSocket, HttpRequest *request) {

    size_t inputSize = strlen(request->body);

    Player *deserializedPlayer = 
        (Player *)base64_decode(request->body, inputSize, &inputSize);

    printf("\nBefore: Player(id=%d, hit=%s)\n\n", deserializedPlayer->id,
           deserializedPlayer->hit == true ? "true" : "false");
    printBytes(deserializedPlayer, sizeof(Player));

    deserializedPlayer->id += 10;

    deserializedPlayer->hit = deserializedPlayer->hit == true ? false : true;

    printf("\nAfter: Player(id=%d, hit=%s)\n\n", deserializedPlayer->id,
           deserializedPlayer->hit == true ? "true" : "false");
    printBytes(deserializedPlayer, sizeof(Player));
    puts("");

    inputSize = sizeof(Player);

    char *encodedBody = 
        base64_encode(deserializedPlayer, inputSize, &inputSize);

    HttpResponse *response = constructHttpResponse(200, NULL, encodedBody);

    respondToHttpRequest(clientSocket, response);

    free(encodedBody);
    free(deserializedPlayer);
    httpRequestDestroy(request);
    httpResponseDestroy(response);
}


// Add a player to the list given from the 
void handleAddPlayer(int clientSocket, HttpRequest *request) {

    size_t objSize = strlen(request->body);

    Player *deserializedPlayer = base64_decode(request->body, objSize, &objSize);

    HttpResponse *response;

    if (numPlayers >= 4) 
        response = constructHttpResponse(400, NULL, "Game is full");
    else {
        players[numPlayers++] = *deserializedPlayer;
        
        char buffer[1024];

        sprintf(buffer, "Successfully added Player(id=%d, hit=%s)",
                deserializedPlayer->id,
                deserializedPlayer->hit == true ? "true" : "false");

        response = constructHttpResponse(200, NULL, buffer);
    }

    printf("\nNumofPlayers: %d\n\n", numPlayers);

    for (int i = 0; i < numPlayers; i++) {
        printf("Player(id=%d, hit=%s)\n",
               players[i].id, players[i].hit == true ? "true" : "false");
    }
    puts("");

    respondToHttpRequest(clientSocket, response);

    free(deserializedPlayer);
    httpRequestDestroy(request);
    httpResponseDestroy(response);
}


void handleGetPlayers(int clientSocket, HttpRequest *request) {

    char buffer[1024];
    memset(buffer, 0, 1024);

    if (numPlayers > 0) {

        for (int i = 0; i < numPlayers; i++) {

            char playerStr[100];

            sprintf(playerStr, "Player(id=%d, hit=%s)\n",
                    players[i].id,
                    players[i].hit == true ? "true" : "false");

            strcat(buffer, playerStr);
        }
    } else {
        sprintf(buffer, "There are no players");
    }

    HttpResponse *response = constructHttpResponse(200, NULL, buffer);

    respondToHttpRequest(clientSocket, response);

    httpRequestDestroy(request);
    httpResponseDestroy(response);
}
