#include "client.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


#include "../lib/base64.h"
#include "../lib/blackJack.h"

#define DEFAULT_PORT 9001
#define LOCALHOST "127.0.0.1"

Player *player;
GameData *gameState;


typedef struct ClientAction {
    char *description;
    void (*action)(HttpClient *client);
} ClientAction;


ClientAction clientActions[10];
int clientActionCount = 0;


void registerClientAction(char *description, void (*action)(HttpClient *client)) {
    clientActions[clientActionCount++].description = description;
    clientActions[clientActionCount++].action = action;
}

bool clientPrompt() {
    char response[20];
    bool processing;
    printf("Hit or stand?\n");
    do {
        scanf("%s", response);
        if (strcmp(response, "hit") == 0) {
            processing = false;
            return true;
        } else if (strcmp(response, "stand") == 0) {
            processing = false;
            return false;
        } else {
            printf("ya silly goose that wasn't an option\n");
            processing = false;
        }
    } while (processing);
    // FIX: Return something? I'm having it return true for now
    return true;
} // end of prompt

// Assigns server ip address and port number based on command line arguments.
// @param argc - number of commandline arguments.
// @param argv - array of commandline arguments.
// @param serverIpAddress - pointer to string containing the server ip address.
// @param port - pointer to int containing target port number.
void assignServerIp(int argc, char *argv[], char **serverIpAddress, int *port) {

    // Use localhost:9001 as default server ip address
    *serverIpAddress = LOCALHOST;
    *port = DEFAULT_PORT;

    if (argc >= 2) *serverIpAddress = argv[1];
    if (argc >= 3) *port = atoi(argv[2]);

    if (port <= 0) {
        perror("Port number must be greater than 0.");
        exit(EXIT_FAILURE);
    }
}

void createNewGame(HttpClient *client) {
    HttpRequest *request = constructHttpRequest(POST, "/create-game", NULL, NULL);
    HttpResponse *response = sendHttpRequest(client, request);

    if( response->statusCode == 200 ){

        size_t inputSize = strlen(response->body);
        player = base64_decode(response->body, inputSize, &inputSize); 


        puts("CREATING NEW GAME");
        puts("you are the host");

    }else{
        puts("game already created, join game instead");
    }
}

void joinGame(HttpClient *client){

    if (player){
        puts("already joined game waiting on host to start");
    }else{
        HttpRequest *request = constructHttpRequest(POST, "/join-game", NULL, NULL);
        HttpResponse *response = sendHttpRequest(client, request);
        if( response ){
            // FIX: Need to check response status code for fail to join
            // like when the game is full. 200=join success, 400=join failed
            // and need to display error message
            // NOTE: The response body contains message from server
            puts("You have joined the game, waiting on host to start");
            size_t inputSize = strlen(response->body);
            player = base64_decode(response->body, inputSize, &inputSize); 
            printf("You are player %d\n",player->id);

            printf("Response body: %s\n", response->body);

        }else{
            puts("failer to recive messgae from server");
        }
    }

    // MOVE SOMEWHERE ELSE
//    size_t inputSize = sizeof(Player);
//    char *message = base64_encode(player, inputSize, &inputSize);
//    HttpRequest *request = constructHttpRequest(POST, "/join-game", NULL, message);
//    HttpResponse *response = sendHttpRequest(client, request);

}

void startGame(HttpClient *client){

    puts("Starting Game");
    printf("player id is : %d\n", player->id);
    if ( player->id == 1){
        char buffer[100];
        sprintf(buffer, "%d", player->id);
        HttpRequest *request = constructHttpRequest(POST, "/start-game", NULL, buffer);
        HttpResponse *response = sendHttpRequest(client, request);
        if (response ){
            printf("%s\n", response->body);
        }else{
            puts("failed to start game");
        }
    }else{
        puts("only the host can start the game");

    }
}

void clientHit(HttpClient *client) {

    // FIX: What does clientPrompt do and where is it defined?
    // It is currently not defined
    if (clientPrompt()){
        player->hit = true;
        player->playing = true;
    }else{
        player->hit = false;
        player->playing = false;
    }

    size_t inputSize = sizeof(Player);
    char *message = base64_encode(player, inputSize, &inputSize);
    HttpRequest *request = constructHttpRequest(POST, "/hit", NULL, message);
    HttpResponse *response = sendHttpRequest(client, request);

    if (response){
        size_t inputSize = strlen(response->body);
        gameState = base64_decode(response->body, inputSize, &inputSize); 
        player = &gameState->players[player->id];
    }else{
        puts("could not get game data from server");
    }
}

void checkGameStatus(HttpClient *client){

    HttpRequest *request = constructHttpRequest(GET, "/game-status", NULL, NULL);
    HttpResponse *response = sendHttpRequest(client, request);
    size_t inputSize = strlen(response->body);
    gameState = base64_decode(response->body, inputSize, &inputSize); 
    player = &(gameState->players[player->id]);
    printf("Player count = %d\n", gameState->playerCount);
    for (int i = 0; i < gameState->playerCount + 1; i++){
        printf("Player %d holds %zu cards\n", i + 1, gameState->players[i + 1].handSize);
        printHand(i, *gameState, 0);
        puts("\n");
    }
}

void registerClientActions() {
    registerClientAction("Create new game", createNewGame);
    registerClientAction("Join Game", joinGame);
    registerClientAction("Start Game", startGame);
    registerClientAction("Take a Turn", clientHit);
    registerClientAction("Check Game Status", checkGameStatus);
}

void printPrompt() {
    for (int i = 0; i < clientActionCount - 1; i++) {
        printf("%d. %s\n", i + 1, clientActions[i].description);
    }
}


int main(int argc, char *argv[]) {
    char *serverIpAddress;
    int port;

    assignServerIp(argc, argv, &serverIpAddress, &port);

    HttpClient *client = constructHttpClient(serverIpAddress, port);

    registerClientActions();

    if (client == NULL) {
        puts("Failed to construct client");
        return 1;
    }

    bool run = true;
    while (run) {

        printPrompt();
        printf("Enter your choice [1-%d]: ", clientActionCount - 1);

        int input = 0;
        scanf("%d", &input);
        getchar();
        puts("");

        // validate input before access
        clientActions[input].action(client);

        printf("\nPress enter to continue...");
        getchar();
    }
}

