#include "blackJack.h"


// stack functions
void shuffle(char **unshuffledDeck) {
  srand(time(NULL));
  int checker = 0;
  int checkerTwo = 0;
  for (int i = 0; i < 1000; i++) {
    checker = (rand() % 52);
    checkerTwo = (rand() % 52);
    char *temp = unshuffledDeck[checker];
    unshuffledDeck[checker] = unshuffledDeck[checkerTwo];
    unshuffledDeck[checkerTwo] = temp;
  }
}

void printHand(int playerId, GameData currentState, int choice) {
  if (playerId == 0 && choice == 1) {
    printf("Dealer's hand: xx");
    for (int i = 1; i < HAND_SIZE; i++) {
      printf(", %s", currentState.players[playerId].hand[i]);
    }
    printf("\n");
  } else {
    printf("Player %d's hand: ", playerId);
    for (int i = 0; i < HAND_SIZE; i++) {
      printf(", %s", currentState.players[playerId].hand[i]);
    }
    printf("\n");
  }
}

// game functions

// adds cards from deck to hand
GameData hit(int playerId, GameData currentState) {
  currentState.players[playerId].hand[currentState.players[playerId].handCounter++] = currentState.deck[currentState.deckCounter++];

  // for (int i = 0; i <= currentState.deckCounter; i++) {
  //   currentState.deck[i] = currentState.deck[i + 1];
  // }

  return currentState;
} // end of hit

// finds the total value of the hand
int checkHandTotal(char *playerHand[]) {
  int handTotal = 0;
  int value = 0;
  int aceCounter = 0;
  for (int x = 0; x < 10; x++) {
    switch (playerHand[x][0]) {
    case 'A':
      aceCounter++;
      value = 11;
      handTotal = handTotal + value;
      break;
    case 'J':
    case 'Q':
    case 'K':
    case 'T':
      value = 10;
      handTotal = handTotal + value;
      break;
    case '2':
      value = 2;
      handTotal = handTotal + value;
      break;
    case '3':
      value = 3;
      handTotal = handTotal + value;
      break;
    case '4':
      value = 4;
      handTotal = handTotal + value;
      break;
    case '5':
      value = 5;
      handTotal = handTotal + value;
      break;
    case '6':
      value = 6;
      handTotal = handTotal + value;
      break;
    case '7':
      value = 7;
      handTotal = handTotal + value;
      break;
    case '8':
      value = 8;
      handTotal = handTotal + value;
      break;
    case '9':
      value = 9;
      handTotal = handTotal + value;
      break;
    default:
      value = 0;
      break;
    } // end switch
  }   // end for
  if (aceCounter > 0) {
    while (handTotal > 21 && aceCounter > 0) {
      handTotal = handTotal - 10;
      aceCounter--;
    }
  }
  if (handTotal > 21) {
    printf("BUST\n");
  }
  return handTotal;
} // end of CheckHandTotal

// resets the hand
char *resetHand(char *playerHand) {
  for (int x = 0; x < 10; x++) {
    playerHand[x] = 0;
  } // end for
} // end of resetHand

// TODO fix issue with the constant loop of you win/ you lose. basically needs a
// way to cap of the function
GameData playDealer(GameData currentState) {
  int dealerTotal = checkHandTotal(currentState.players[0].hand);

  // printHand(0, currentState, 0);
  convertAndPrint(currentState.players[0].hand, HAND_SIZE);
  while (dealerTotal < 17 && dealerTotal > !21) {
    currentState = hit(0, currentState); // dealer hits
    dealerTotal = checkHandTotal(currentState.players[0].hand);
    printf("dealer hits, dealer: %d\n", dealerTotal);
  }

  // start at 1 becasue 0 is the dealer
  for (int i = 1; i < currentState.playerCount; i++) {
    currentState.players[i].total =
        checkHandTotal(currentState.players[i].hand);
    printf("\nPlayer %d:\n", i);
    // printHand(currentState.players[i].id, currentState, 0);
    convertAndPrint(currentState.players[i].hand, HAND_SIZE);

    if (dealerTotal > 21 ||
        (currentState.players[i].total == 21 && dealerTotal != 21)) {
      printf("  Player%d YOU WIN\n", i);
    } else if (currentState.players[i].total > 21) {
      printf(" Player%d YOU LOSE\n", i);
    } else if (currentState.players[i].total > dealerTotal) {
      printf(" player%d YOU WIN\n", i);
    } else {
      printf(" player%d, YOU LOSE\n", i);
    }
  }
  return currentState;
}

GameData Prompt(int playerId, GameData currentState) {
  char response[20];
  bool processing;
  printf("Player %d, hit or stand?\n", playerId);
  do {
    scanf("%s", response);
    if (strcmp(response, "hit") == 0) {
      currentState = hit(playerId, currentState);
      printHand(currentState.players[playerId].id, currentState, 0);
      processing = false;
    } else if (strcmp(response, "stand") == 0) {
      currentState.players[playerId].playing = false;
      processing = false;
    } else {
      printf("ya silly goose that wasn't an option\n");
    }
  } while (processing);
  return currentState;
} // end of prompt

bool clientPrompt() {
  char response[20];
  bool processing;
  printf("Hit or stand?\n");
  do {
    scanf("%s", response);
    if (strcmp(response, "hit") == 0) {
      // ADD SERVER
      // send server "hit == true" 
      // get new hand from server
      convertAndPrint(newHand, HAND_SIZE);
      processing = false;
      return true;
    } else if (strcmp(response, "stand") == 0) {
      // ADD SERVER
      // send server "hit == false"
      processing = false;
      return false;
    } else {
      printf("ya silly goose that wasn't an option\n");
    }
  } while (processing);
} // end of prompt

GameData serverPrompt(int playerId, GameData currentState){
  // ADD SERVER
  //send prompt to client
  //recive answer from clinet
  bool responseHit;
  if (responseHit){
    currentState = hit(playerId, currentState);
    printHand(currentState.players[playerId].id, currentState, 0);\
    // ADD SERVER
    // send player their hand to print
  }else{
    currentState.players[playerId].playing = false;
  }
  return currentState;
}


GameData initializePlayers(GameData gameState){
  char *baseDeck[DECK_SIZE] = {
      "Ac", "2c", "3c", "4c", "5c", "6c", "7c", "8c", "9c", "Tc", "Jc",
      "Qc", "Kc", "Ad", "2d", "3d", "4d", "5d", "6d", "7d", "8d", "9d",
      "Td", "Jd", "Qd", "Kd", "Ah", "2h", "3h", "4h", "5h", "6h", "7h",
      "8h", "9h", "Th", "Jh", "Qh", "Kh", "As", "2s", "3s", "4s", "5s",
      "6s", "7s", "8s", "9s", "Ts", "Js", "Qs", "Ks",
  };
  char *emptyHand1[HAND_SIZE] = {"  ", "  ", "  ", "  ", "  ", "  ",
                                 "  ", "  ", "  ", "  ", "  "};
  char *emptyHand2[HAND_SIZE] = {"  ", "  ", "  ", "  ", "  ", "  ",
                                 "  ", "  ", "  ", "  ", "  "};
  char *emptyHand3[HAND_SIZE] = {"  ", "  ", "  ", "  ", "  ", "  ",
                                 "  ", "  ", "  ", "  ", "  "};
  char *emptyHand4[HAND_SIZE] = {"  ", "  ", "  ", "  ", "  ", "  ",
                                 "  ", "  ", "  ", "  ", "  "};
  char *emptyHand5[HAND_SIZE] = {"  ", "  ", "  ", "  ", "  ", "  ",
                                 "  ", "  ", "  ", "  ", "  "};

  gameState.deck = baseDeck;
  shuffle(gameState.deck);
  gameState.deckCounter = 0;

  // server determines numer of players
  gameState.playerCount = 3; // hardcoded fro now
  gameState.playerCount++;   // for dealer

  // intialize players
  for (int i = 0; i < gameState.playerCount + 1; i++) {
    gameState.players[i].playing = true;
    gameState.players[i].handCounter = 0;
    gameState.players[i].id = i;
  }
  gameState.players[0].hand = emptyHand1;
  gameState.players[1].hand = emptyHand2;
  gameState.players[2].hand = emptyHand3;
  gameState.players[3].hand = emptyHand4;
  gameState.players[4].hand = emptyHand5;

  gameState.players[0].playing = false; // makes the game work
  return gameState;
}

void readyGame() {}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// main

void runServerBlackJack(){
  GameData gameState;
  gameState = initializePlayers(gameState);

  for (int x = 0; x < gameState.playerCount; x++) {
    gameState = hit(x, gameState);
    gameState = hit(x, gameState);
  }
  // ADD SERVER
  // 1 send first hand
  
  for (int x = 0; x < gameState.playerCount; x++) {
    //printf("Player %d\n", gameState.players[x].id);
    convertAndPrint(gameState.players[x].hand, HAND_SIZE);
    // printHand(x, gameState, 1);
  }

    gameState.playersTurn = true;
  while (gameState.playersTurn) {

    for (int i = 1; i < gameState.playerCount; i++) {

      if (gameState.players[i].playing)
        gameState = serverPrompt(i, gameState);
    }

    for (int i = 0; i < gameState.playerCount; i++) {
        if(gameState.players[i].playing == false){
          countFinishedPlayer++;
        }
    }
    printf(" count finished players %d\n", countFinishedPlayer);
    if (countFinishedPlayer == gameState.playerCount){
    gameState.playersTurn = false;
    }else{
      countFinishedPlayer = 0;
    }
  }
  playDealer(gameState);

  // ADD SERVER
  // send final hands
}

void runClinetBlackJack(){
  // ADD SERVER
  //  1 recive first hand and dealers hand from server
  convertAndPrint(firstHand, HAND_SIZE);

  bool hitting;
  while(hitting){
    // ADD SERVER
    // wait for info to execute prompt
    hitting = clientPrompt();
  }

  //send dealer hand
  dealerTotal = checkHandTotal(dealerHand);
  //send player hand
  playerTotal = checkHandTotal(playerHand;);
  
  if (dealerTotal > 21 || (currentState.players[i].total == 21 && dealerTotal != 21)) {
      printf("  Player%d YOU WIN\n", i);
    } else if (currentState.players[i].total > 21) {
      printf(" Player%d YOU LOSE\n", i);
    } else if (currentState.players[i].total > dealerTotal) {
      printf(" player%d YOU WIN\n", i);
    } else {
      printf(" player%d, YOU LOSE\n", i);
    }
  }
  
}


int playGame(void) {

  GameData gameState;
  gameState = initializePlayers(gameState);

  for (int x = 0; x < gameState.playerCount; x++) {
    gameState = hit(x, gameState);
    gameState = hit(x, gameState);
  }

  // values must be sent to players to be printed by players
  // for (int i = 0; i < gameState.playerCount + 1; i++) {
  //   printHand(gameState.players[i]);
  // }

  // int countFinishedPlayer;
  
  for (int x = 0; x < gameState.playerCount; x++) {
    //printf("Player %d\n", gameState.players[x].id);
    convertAndPrint(gameState.players[x].hand, HAND_SIZE);
    // printHand(x, gameState, 1);
  }

  gameState.playersTurn = true;
  while (gameState.playersTurn) {

    for (int i = 1; i < gameState.playerCount; i++) {

      // call client
      if (gameState.players[i].playing)
        gameState = prompt(i, gameState);
    }

    for (int i = 0; i < gameState.playerCount; i++) {
        if(gameState.players[i].playing == false){
          countFinishedPlayer++;
        }
    }
    if (countFinishedPlayer == gameState.playerCount){
    gameState.playersTurn = false;
    }else{
      countFinishedPlayer = 0;
    }
  }

  playDealer(gameState);

  return 0;
}

int main(void) {
  playGame();
  return 0;
}

