#include "blackJack.h"

#include <stddef.h>
#include <stdlib.h>


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
    
    displayCards(currentState.players[playerId].hand, currentState.players[playerId].handSize);
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


void displayCards(Card hand[], size_t amountOfCards) {
// void displayCards(Card handToRead[], size_t handToReadLength) {
    Card emptyCard;
    printf("%s", "\n");
    int linesInCard = 4;
    //int handToReadLength = sizeof(handToRead) / sizeof(* handToRead);   // length of array
    //int handToReadLength = 11;
    for (int currentLine = 0; currentLine < linesInCard; ++currentLine) {
        printf("%s", "\n");
        //displayCardLine(handToRead, handToReadLength, counter);
        for (int counter = 0; counter < amountOfCards; ++counter) {
            if (hand[counter].face != emptyCard.face) {
                switch (currentLine) {
                    case 0:
                        printf(" %s", "._____.");
                        break;
                    case 1:
                        printf(" %s%c%s", "|  ", hand[counter].face, "  |");
                        break;
                    case 2:
                        printf(" %s%c%s", "|  ", hand[counter].suit, "  |");
                        break;
                    case 3:
                        printf(" %s", "._____.");
                        break;
                    default:
                        printf(" %s", "xxxxxxx");
                        break;
                }   // end switch
            } else {
                counter = amountOfCards;
            }   // end if
        }   // end for
    }
    printf("%s", "\n");
}   // end displayCards


//########## Game Functions ##########//

// adds cards from deck to hand
GameData hit(int playerId, GameData currentState) {
    puts("start hit");
    currentState.players[playerId].hand[currentState.players[playerId].handCounter] =
        currentState.deck[currentState.deckCounter++];

    currentState.players[playerId].handCounter++;
    currentState.players[playerId].handSize++;
    // for (int i = 0; i <= currentState.deckCounter; i++) {
    //   currentState.deck[i] = currentState.deck[i + 1];
    // }
    puts("end hit");
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
    printHand(0, currentState, 0);
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





/////////////////////////////////////////////////////////////////////////////////////////////////////
// main


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

