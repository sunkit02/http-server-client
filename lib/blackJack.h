#ifndef BLACK_JACK_H
#define BLACK_JACK_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "displayCards.h"

#define DECK_SIZE 52
#define HAND_SIZE 11
#define MAX_PLAYERS 4

// char baseDeck[DECK_SIZE];
int countFinishedPlayer;

typedef struct player{
  char **hand;
  int total;
  int handCounter;
  int id;
  bool hit;
  bool playing;
}Player;



typedef struct allGameData{
  Player players[MAX_PLAYERS + 1]; // + 1 for dealer
  int playerCount;
  char** deck;
  int deckCounter;
  bool playersTurn;
}GameData;


//funtions
GameData initializePlayers(GameData gameState);

void shuffle(char **unshuffledDeck);
void ready_play_deck(char **shuffledDeck);

void printHand(int playerId, GameData currentState, int choice);
GameData hit(int playerId, GameData currentState);
int checkHandTotal(char *playerHand[]);
char* resetHand(char* playerHand);
GameData playDealer(GameData currentState);

bool clientPrompt();
GameData serverPrompt(int playerId, GameData currentState);
GameData prompt(int playerId, GameData currentState);

void runClinetBlackJack();
void runServerBlackJack();
int playGame(void);

#endif
