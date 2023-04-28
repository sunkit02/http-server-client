#include "blackJack.h"
#include "base64.h"
#include "http.h"
#include "../server/server.h"

#include <stddef.h>
#include <stdlib.h>

GameData gameData;


// stack functions
void shuffle(Card *unshuffledDeck) {
    srand(time(NULL));
    int checker = 0;
    int checkerTwo = 0;
    for (int i = 0; i < 1000; i++) {
        checker = (rand() % 52);
        checkerTwo = (rand() % 52);
        Card temp = unshuffledDeck[checker];
        unshuffledDeck[checker] = unshuffledDeck[checkerTwo];
        unshuffledDeck[checkerTwo] = temp;
    }
}

void printHand(int playerId, GameData currentState, int choice) {
    // if (playerId == 0 && choice == 1) {
    //     printf("Dealer's hand: xx");
    //     for (int i = 1; i < HAND_SIZE; i++) {
    //         printf(", %s", currentState.players[playerId].hand[i]);
    //     }
    //     printf("\n");
    // } else {
    //     printf("Player %d's hand: ", playerId);
    //     for (int i = 0; i < HAND_SIZE; i++) {
    //         printf(", %s", currentState.players[playerId].hand[i]);
    //     }
    //     printf("\n");
    // }
    
    convertAndPrint(currentState.players[playerId].hand,
                    currentState.players[playerId].handSize);
}

//######### Display functions ########//

// ASCII Card Graphics test.
/*
 * each card looks something like this:
 *      ._____.
 *      |  5  |
 *      |  H  |
 *      ._____.
 *
 *      Several cards in hand should look something like this:
 *
 *      ._____. ._____. ._____. ._____.
 *      |  A  | |  J  | |  2  | |  4  |
 *      |  S  | |  H  | |  C  | |  D  |
 *      ._____. ._____. ._____. ._____.
 *
 *      Values: A, 2, 3, 4, 5, 6, 7, 8, 9, X, J, Q, K
 *          stand for Ace, Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King
 *      Suits: H, D, C, S
 *          stand for Hearts, Diamonds, Clubs, Spades
 */

// Takes the cards in a player's hand and displays them.


CardArrayWrapper constructCardArray(size_t initialCapacity) {
    CardArrayWrapper arrayWrapper;

    Card *tempPtr = calloc(initialCapacity, sizeof(Card));

    if (tempPtr == NULL) {
        puts("Failed to allocate memory.");
        exit(1);
    }

    arrayWrapper.elementArray = tempPtr;
    arrayWrapper.capacity = initialCapacity;
    arrayWrapper.size = 0;

    return arrayWrapper;
}

void addToCardArray(CardArrayWrapper *arrayWrapper, Card card) {
    // Determine if size is less than capacity
    if (arrayWrapper->size >= arrayWrapper->capacity) {
        // If size is not less than capacity
        puts("Array is full. This theoretically shouldn't be possible.");
        exit(1);
    }

    // Assign the card to index in array
    arrayWrapper->elementArray[arrayWrapper->size++] = card;
}

void clearCardsInHand(CardArrayWrapper *arrayWrapper) {
    arrayWrapper->size = 0;
}

//TODO: Stack from Brandon
// Card popCard(CardArrayWrapper *arrayWrapper) {
//     if (arrayWrapper->size) {}//TODO: make sure the stack isn't empty. If it is, exit program.
// }

void convertAndPrint(Card *hand, size_t length) {
    CardArrayWrapper cardArrayWrapper = constructCardArray(length);
    for (size_t i = 0; i < length; i++) {
        addToCardArray(&cardArrayWrapper, hand[i]);
    }
    displayCards(cardArrayWrapper);
};

void displayCards(CardArrayWrapper hand) {
// void displayCards(Card handToRead[], size_t handToReadLength) {
    Card emptyCard;
    printf("%s", "\n");
    int linesInCard = 4;
    //int handToReadLength = sizeof(handToRead) / sizeof(* handToRead);   // length of array
    //int handToReadLength = 11;
    for (int currentLine = 0; currentLine < linesInCard; ++currentLine) {
        printf("%s", "\n");
        //displayCardLine(handToRead, handToReadLength, counter);
        for (int counter = 0; counter < hand.size; ++counter) {
            if (hand.elementArray[counter].face != emptyCard.face) {
                switch (currentLine) {
                    case 0:
                        printf(" %s", "._____.");
                        break;
                    case 1:
                        printf(" %s%c%s", "|  ", hand.elementArray[counter].face, "  |");
                        break;
                    case 2:
                        printf(" %s%c%s", "|  ", hand.elementArray[counter].suit, "  |");
                        break;
                    case 3:
                        printf(" %s", "._____.");
                        break;
                    default:
                        printf(" %s", "xxxxxxx");
                        break;
                }   // end switch
            } else {
                counter = hand.size;
            }   // end if
        }   // end for
    }
    printf("%s", "\n");
}   // end displayCards


//########## Game Functions ##########//

// adds cards from deck to hand
GameData hit(int playerId, GameData currentState) {
    currentState.players[playerId].hand[currentState.players[playerId].handCounter++] =
        currentState.deck[currentState.deckCounter++];

    currentState.players[playerId].handSize++;
    // for (int i = 0; i <= currentState.deckCounter; i++) {
    //   currentState.deck[i] = currentState.deck[i + 1];
    // }

    return currentState;
} // end of hit

// finds the total value of the hand
int checkHandTotal(Card *playerHand) {
    int handTotal = 0;
    int value = 0;
    int aceCounter = 0;
    for (int i = 0; i < 10; i++) {
        switch (playerHand[i].face) {
            case 'A':
                aceCounter++;
                value = 11;
                handTotal = handTotal + value;
                break;
            case 'J':
            case 'Q':
            case 'K':
            case 'X':
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
        printHand(currentState.players[i].id, currentState, 0);
        // convertAndPrint(currentState.players[i].hand, HAND_SIZE);

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
             processing = false;
             return true;
         } else if (strcmp(response, "stand") == 0) {
             processing = false;
             return false;
         } else {
             printf("ya silly goose that wasn't an option\n");
         }
     } while (processing);
 } // end of prompt

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


GameData initializeGame(GameData gameState){

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


/////////////////////////////////////////////////////////////////////////////////////////////////////
// main

void runServerBlackJack(){
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

// void runClinetBlackJack(){
//     // ADD SERVER
//     //  1 recive first hand and dealers hand from server
//     convertAndPrint(firstHand, HAND_SIZE);
//
//     bool hitting;
//     while(hitting){
//         // ADD SERVER
//         // wait for info to execute prompt
//         hitting = clientPrompt();
//     }
//
//     //send dealer hand
//     dealerTotal = checkHandTotal(dealerHand);
//     //send player hand
//     playerTotal = checkHandTotal(playerHand;);
//
//     if (dealerTotal > 21 || (currentState.players[i].total == 21 && dealerTotal != 21)) {
//         printf("  Player%d YOU WIN\n", i);
//     } else if (currentState.players[i].total > 21) {
//         printf(" Player%d YOU LOSE\n", i);
//     } else if (currentState.players[i].total > dealerTotal) {
//         printf(" player%d YOU WIN\n", i);
//     } else {
//         printf(" player%d, YOU LOSE\n", i);
//     }
// }
//
// }


// int playGame(void) {
//
//   GameData gameState;
//   gameState = initializeGame(gameState);
//
//   for (int x = 0; x < gameState.playerCount; x++) {
//     gameState = hit(x, gameState);
//     gameState = hit(x, gameState);
//   }
//
//   // values must be sent to players to be printed by players
//   // for (int i = 0; i < gameState.playerCount + 1; i++) {
//   //   printHand(gameState.players[i]);
//   // }
//
//   // int countFinishedPlayer;
//   
//   for (int x = 0; x < gameState.playerCount; x++) {
//     //printf("Player %d\n", gameState.players[x].id);
//     convertAndPrint(gameState.players[x].hand, HAND_SIZE);
//     // printHand(x, gameState, 1);
//   }
//
//   gameState.playersTurn = true;
//   while (gameState.playersTurn) {
//
//     for (int i = 1; i < gameState.playerCount; i++) {
//
//       // call client
//       if (gameState.players[i].playing)
//         gameState = prompt(i, gameState);
//     }
//
//     for (int i = 0; i < gameState.playerCount; i++) {
//         if(gameState.players[i].playing == false){
//           countFinishedPlayer++;
//         }
//     }
//     if (countFinishedPlayer == gameState.playerCount){
//     gameState.playersTurn = false;
//     }else{
//       countFinishedPlayer = 0;
//     }
//   }
//
//   playDealer(gameState);
//
//   return 0;
// }

// int main(void) {
//   playGame();
//   return 0;
// }

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
    } else {
        response = constructHttpResponse(400, NULL, "You are not the host");
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
