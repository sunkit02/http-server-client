#ifndef BLACK_JACK_H
#define BLACK_JACK_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#include "../lib/http.h"

#define DECK_SIZE 52
#define HAND_SIZE 11
#define MAX_PLAYERS 4
#define HOST_PLAYER_ID 1

typedef enum {
    WAITING, FULL, PLAYING, ENDED
} GameStatus;

typedef struct Card {
    char face;
    char suit;
} Card;

typedef struct CardArrayWrapper {
    Card *elementArray;
    size_t size;        // current size
    size_t capacity;    // max size
} CardArrayWrapper;

typedef struct Player {
    Card hand[HAND_SIZE];
    size_t handSize;
    int total;
    int handCounter;
    int id;
    bool hit;
    bool playing;
} Player;



typedef struct GameData {
    Player players[MAX_PLAYERS + 1]; // + 1 for dealer
    int playerCount;
    Card deck[DECK_SIZE];
    int deckCounter;
    bool playersTurn;
    GameStatus currentGameStatus;
    int currentPlayerId;
} GameData;


GameData initializePlayers(GameData gameState);

void shuffle(Card *unshuffledDeck);
// void ready_play_deck(Card *shuffledDeck);

void printHand(int playerId, GameData currentState, int choice);
GameData hit(int playerId, GameData currentState);
int checkHandTotal(Card *playerHand);
char* resetHand(char* playerHand);
GameData playDealer(GameData currentState);

bool clientPrompt();
GameData serverPrompt(int playerId, GameData currentState);
GameData prompt(int playerId, GameData currentState);

void runClinetBlackJack();
void runServerBlackJack();
int playGame(void);

//####### Display Functions #######//
CardArrayWrapper constructCardArray(size_t initialCapacity);

void addToCardArray(CardArrayWrapper *arrayWrapper, Card card);

// Clears all the cards stored and resets the arrayWrapper
void clearCardsInHand(CardArrayWrapper *arrayWrapper);

//void cardAssign(Card *card, char face, char suit);
void displayCardLine(Card handToRead[], int handToReadLength, int currentLine);
//void displayCards(Card handToRead[], size_t handToReadLength);
void displayCards(CardArrayWrapper hand);

void convertAndPrint(Card *hand, size_t length);

// ####### SERVER FUNTIONS ######## //
void handleCreateGame(int clientSocket, HttpRequest *request);
void handleJoinGame(int clientSocket, HttpRequest *request);
void handleStartGame(int clientSocket, HttpRequest *request);
void handleHit(int clientSocket, HttpRequest *request);
void handleCheckGameStatus(int clientSocket, HttpRequest *request);

#endif
