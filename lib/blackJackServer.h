#ifndef BLACK_JACK_SERVER_H
#define BLACK_JACK_SERVER_H

#include "blackJack.h"
#include "../lib/http.h"

GameData initializeGame(GameData gameState);

// ####### SERVER FUNTIONS ######## //
void handleCreateGame(int clientSocket, HttpRequest *request);
void handleJoinGame(int clientSocket, HttpRequest *request);
void handleStartGame(int clientSocket, HttpRequest *request);
void handleHit(int clientSocket, HttpRequest *request);
void handleCheckGameStatus(int clientSocket, HttpRequest *request);

#endif // !DEBUG
