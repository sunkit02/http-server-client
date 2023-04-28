#include "../server/server.h"
#include "blackJack.h"
#include "blackJackServer.h"
#include "base64.h"
#include "http.h"
#include <stdio.h>

GameData gameData;


GameData serverPrompt(int playerId, GameData currentState){
    bool responseHit;
    if (responseHit){
        currentState = hit(playerId, currentState);
        printHand(currentState.players[playerId].id, currentState, 0);
    } else {
        currentState.players[playerId].playing = false;
    }
    return currentState;
}

GameData initializeGame(GameData gameState) {

    Card baseDeck[DECK_SIZE] = {
        {'A', 'C'}, {'2', 'C'}, {'3', 'C'}, {'4', 'C'}, {'5', 'C'}, {'6', 'C'}, {'7', 'C'}, {'8', 'C'}, {'9', 'C'}, {'X', 'C'}, {'J', 'C'}, {'Q', 'C'}, {'K', 'C'},
        {'A', 'H'}, {'2', 'H'}, {'3', 'H'}, {'4', 'H'}, {'5', 'H'}, {'6', 'H'}, {'7', 'H'}, {'8', 'H'}, {'9', 'H'}, {'X', 'H'}, {'J', 'H'}, {'Q', 'H'}, {'K', 'H'},
        {'A', 'D'}, {'2', 'D'}, {'3', 'D'}, {'4', 'D'}, {'5', 'D'}, {'6', 'D'}, {'7', 'D'}, {'8', 'D'}, {'9', 'D'}, {'X', 'D'}, {'J', 'D'}, {'Q', 'D'}, {'K', 'D'},
        {'A', 'S'}, {'2', 'S'}, {'3', 'S'}, {'4', 'S'}, {'5', 'S'}, {'6', 'S'}, {'7', 'S'}, {'8', 'S'}, {'9', 'S'}, {'X', 'S'}, {'J', 'S'}, {'Q', 'S'}, {'K', 'S'},
    };

    for (int i= 0; i < DECK_SIZE; i++){
        gameState.deck[i] = baseDeck[i];
    }

    shuffle(gameState.deck);
    gameState.deckCounter = 0;

    // server determines numer of players
    gameState.playerCount = 0;

    // intialize players
    for (int i = 0; i < gameState.playerCount + 1; i++){
        hit(i, gameState);
        hit(i, gameState);
    }

    gameState.players[0].playing = false; // makes the game work
    return gameState;
}


void runServerBlackJack() {
    gameData = initializeGame(gameData);

    for (int x = 0; x < gameData.playerCount; x++) {
        gameData = hit(x, gameData);
        gameData = hit(x, gameData);
    }
    // ADD SERVER
    // 1 send first hand

    for (int x = 0; x < gameData.playerCount; x++) {
        //printf("Player %d\n", gameData.players[x].id);
        convertAndPrint(gameData.players[x].hand, HAND_SIZE);
        // printHand(x, gameData, 1);
    }

    gameData.playersTurn = true;
    // while (gameData.playersTurn) {
    //
    //   for (int i = 1; i < gameData.playerCount; i++) {
    //
    //     if (gameData.players[i].playing)
    //       gameData = serverPrompt(i, gameData);
    //   }
    //
    //   for (int i = 0; i < gameData.playerCount; i++) {
    //       if(gameData.players[i].playing == false){
    //         countFinishedPlayer++;
    //       }
    //   }
    //   printf(" count finished players %d\n", countFinishedPlayer);
    //   if (countFinishedPlayer == gameData.playerCount){
    //   gameData.playersTurn = false;
    //   }else{
    //     countFinishedPlayer = 0;
    //   }
    // }
    playDealer(gameData);

    // ADD SERVER
    // send final hands
}




// ####### SERVER FUNCTIONS ######## //

void handleCreateGame(int clientSocket, HttpRequest *request) {

    HttpResponse *response;

    if (gameData.playerCount == 0) {
        // Add host to players
        Player host;
        host.id = HOST_PLAYER_ID;
        host.handSize = 0;

        // offset by 1 for dealer
        gameData.players[1 + gameData.playerCount++] = host;
        gameData.currentGameStatus = WAITING;

        size_t inputSize = sizeof(Player);
        char *hostSerialized = base64_encode(&host, inputSize, &inputSize);

        response = constructHttpResponse(200, NULL, hostSerialized);

        free(hostSerialized);
    } else {
        response = constructHttpResponse(400, NULL, "Game already created");
    }

    respondToHttpRequest(clientSocket, response);

    httpRequestDestroy(request);
    httpResponseDestroy(response);
}

void handleJoinGame(int clientSocket, HttpRequest *request) {
    HttpResponse *response;

    if (gameData.currentGameStatus == WAITING) {
        Player player;
        player.id = gameData.playerCount;
        player.handSize = 0;
        player.playing = true;

        // offset by 1 for dealer
        gameData.players[1 + gameData.playerCount++] = player;

        if (gameData.playerCount >= MAX_PLAYERS) {
            gameData.currentGameStatus = FULL;
            puts("players maxed out cannot add more");
        }
        size_t inputSize = sizeof(Player);
        char *playerSerialized = base64_encode(&player, inputSize, &inputSize);

        response = constructHttpResponse(200, NULL, playerSerialized);

        free(playerSerialized);
    } else {
        response = constructHttpResponse(400, NULL, "Game already full");
    }

    respondToHttpRequest(clientSocket, response);

    httpRequestDestroy(request);
    httpResponseDestroy(response);
}

void handleStartGame(int clientSocket, HttpRequest *request) {
    HttpResponse *response;
    // Require player id to be sent in request body
    int playerId = atoi(request->body);

    if (playerId == HOST_PLAYER_ID) {
        // Start game
        initializeGame(gameData);
        response = constructHttpResponse(200, NULL, "Game started");
        printf("game started\n");
        gameData.currentGameStatus = PLAYING;
    } else {
        response = constructHttpResponse(400, NULL, "You are not the host");
        puts(" DEBUGGING MODE GAME STARTS ANYWAYS ");
        gameData.currentGameStatus = PLAYING;
    }

    respondToHttpRequest(clientSocket, response);

    httpRequestDestroy(request);
    httpResponseDestroy(response);
}

void handleHit(int clientSocket, HttpRequest *request) {
    HttpResponse *response;
    char * gameStateEncoded = NULL;
    // require hit or not in request body
    if (gameData.currentGameStatus == PLAYING) {

        size_t inputSize = strlen(request->body);
        Player *player = base64_decode(request->body, inputSize, &inputSize);

        if (gameData.currentPlayerId == player->id) {

            gameData = serverPrompt(player->id, gameData);

            size_t inputSize = sizeof(GameData);
            gameStateEncoded =  base64_encode(&gameData, inputSize, &inputSize);
            response = constructHttpResponse(200, NULL, gameStateEncoded);
        } else {
            response = constructHttpResponse(400, NULL, "It is not your turn");
        }

    } else {
        response = constructHttpResponse(400, NULL, "Game is not in progress");
    }

    // handle all stay
    bool allStay = true;
    for (int i = 1; i < gameData.playerCount; i++) {
        if (gameData.players[i].playing == true) {
            allStay = false;
            break;
        }
    }

    if (allStay) {
        // dealer play
        gameData = playDealer(gameData);
        gameData.currentGameStatus = ENDED;
    }

    respondToHttpRequest(clientSocket, response);

    free(gameStateEncoded);
    httpRequestDestroy(request);
    httpResponseDestroy(response);
}

void handleCheckGameStatus(int clientSocket, HttpRequest *request) {
    size_t inputSize = sizeof(GameData);
    char *gameDataEncoded = base64_encode(&gameData, inputSize, &inputSize);

    HttpResponse *response = constructHttpResponse(200, NULL, gameDataEncoded);

    respondToHttpRequest(clientSocket, response);

    free(gameDataEncoded);
    httpRequestDestroy(request);
    httpResponseDestroy(response);
}
