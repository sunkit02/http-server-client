//initializing for the whole code
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define SIZE 52
#define HAND 11
bool player1=true;
bool player2=true;
bool player3=true;
bool player4=true;

int falseCount=0;
int playerCount;
int playerTurn=1;
char handTotal;
char dealerTotal;
int handCounter1 = 0;
int handCounter2 = 0;
int handCounter3 = 0;
int handCounter4 = 0;
int dealerHandCounter = 0;
int first = -1;

char *play_deck[SIZE];
char *deck[SIZE] = {"Ac", "2c", "3c", "4c", "5c", "6c", "7c", "8c", "9c", "Tc", "Jc", "Qc", "Kc",
                   "Ad", "2d", "3d", "4d", "5d", "6d", "7d", "8d", "9d", "Td", "Jd", "Qd", "Kd",
                   "Ah", "2h", "3h", "4h", "5h", "6h", "7h", "8h", "9h", "Th", "Jh", "Qh", "Kh",
                   "As", "2s", "3s", "4s", "5s", "6s", "7s", "8s", "9s", "Ts", "Js", "Qs", "Ks",};
char *hand[HAND] = {"0", "0", "0", "0", "0", "0", "0", "0", "0", "0","0"};
char *hand2[HAND] = {"1", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"};
char *hand3[HAND] = {"0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"};
char *hand4[HAND] = {"0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"};
char *dealerHand[HAND] = {"0", "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"};




/////////////////////////////////////////////////////////////////////////////////////////////////////
//stack functions
void shuffle() {
   srand(time(NULL));
   int checker = 0;
   int checkerTwo = 0;
   char *temp;
   temp = "0";
   for (int i = 0; i < 1000; i++) {
       checker = (rand() % 52);
       checkerTwo = (rand() % 52);
       temp = deck[checker];
       deck[checker] = deck[checkerTwo];
       deck[checkerTwo] = temp;
   }
}

//sets of the deck as a stack
void ready_play_deck() {
   for (int i = 0; i < 52; i++) {
       first += 1;
       for (int j = first; j >= 0; j--) {
           play_deck[j] = play_deck[j - 1];
       }
       play_deck[0] = deck[i];
   }
}

//void print_play_deck() {
//   for (int z = 0; z < 52; z++) {
//       printf("%s, ", play_deck[z]);
//   }
//   printf("\n\n");
//}
//void print_deck() {
//   for (int z = 0; z < 52; z++) {
//       printf("%s, ", deck[z]);
//   }
//   printf("\n\n");
//}

//shows the hand to the player
void print_hand(int player) {
    switch(player) {
        case 1:
            printf("Player 1's Hand: ");
            for (int z = 0; z < 10; z++) {
                printf("%s, ", hand[z]);
            }
            printf("\n");
            break;
        case 2:
            printf("Player 2's Hand: ");
            for (int z = 0; z < 10; z++) {
                printf("%s, ", hand2[z]);
            }
            printf("\n");
            break;
        case 3:
            printf("Player 3's Hand: ");
            for (int z = 0; z < 10; z++) {
                printf("%s, ", hand3[z]);
            }
            printf("\n");
            break;
        case 4:
            printf("Player 4's Hand: ");
            for (int z = 0; z < 10; z++) {
                printf("%s, ", hand4[z]);
            }
            printf("\n");
            break;
    }

}

//shows the dealers hand to the player
void print_dealerHand() {
   printf("Dealer's Hand: ");
   for (int z = 0; z < 10; z++) {
       printf("%s, ", dealerHand[z]);
   }
   printf("\n");
}




/////////////////////////////////////////////////////////////////////////////////////////////////////
//game functions


//adds cards from deck to hand
void hit(int player) {
   switch (player){
       case 1:
           hand[handCounter1] = play_deck[0];
           handCounter1++;
           break;
       case 2:
           hand2[handCounter2] = play_deck[0];
           handCounter2++;
           break;
       case 3:
           hand3[handCounter3] = play_deck[0];
           handCounter3++;
           break;
       case 4:
           hand4[handCounter4] = play_deck[0];
           handCounter4++;
           break;
   }
   for (int i = 0; i <= first; i++) {
       play_deck[i] = play_deck[i + 1];
   }
   first -= 1;


}//end of hit

//adds card from deck to dealers hand
void dealerHit() {
   dealerHand[dealerHandCounter] = play_deck[0];
   for (int i = 0; i <= first; i++) {
       play_deck[i] = play_deck[i + 1];
   }
   first -= 1;
   dealerHandCounter++;
}//end of hit



//TODO fix checkHandTotal so that it works better with the int player format

// finds the total value of the hand
void checkHandTotal(char *data[]) {
   handTotal = 0;
   int value = 0;
   int aceCounter = 0;
   for (int x = 0; x < 10; x++) {
       switch (data[x][0]) {
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
       }//end switch
   }//end for
   if (aceCounter > 0) {
       while (handTotal > 21 && aceCounter > 0) {
           handTotal = handTotal - 10;
           aceCounter--;
       }
   }
   data[10]= handTotal;

   if (handTotal > 21) {
       printf("BUST\n");
   }
}//end of CheckHandTotal


//checks the value of the dealers hand
void CheckDealerTotal() {
   dealerTotal = 0;
   int DealerValue = 0;
   int aceCounter = 0;
   for (int x = 0; x < 10; x++) {
       switch (dealerHand[x][0]) {
           case 'A':
               aceCounter++;
               DealerValue = 11;
               dealerTotal = dealerTotal + DealerValue;
               break;
           case 'J':
           case 'Q':
           case 'K':
           case 'T':
               DealerValue = 10;
               dealerTotal = dealerTotal + DealerValue;
               break;
           case '2':
               DealerValue = 2;
               dealerTotal = dealerTotal + DealerValue;
               break;
           case '3':
               DealerValue = 3;
               dealerTotal = dealerTotal + DealerValue;
               break;
           case '4':
               DealerValue = 4;
               dealerTotal = dealerTotal + DealerValue;
               break;
           case '5':
               DealerValue = 5;
               dealerTotal = dealerTotal + DealerValue;
               break;
           case '6':
               DealerValue = 6;
               dealerTotal = dealerTotal + DealerValue;
               break;
           case '7':
               DealerValue = 7;
               dealerTotal = dealerTotal + DealerValue;
               break;
           case '8':
               DealerValue = 8;
               dealerTotal = dealerTotal + DealerValue;
               break;
           case '9':
               DealerValue = 9;
               dealerTotal = dealerTotal + DealerValue;
               break;
           default:
               DealerValue = 0;
               break;
       }//end switch
   }//end for
   if (aceCounter > 0) {
       while (dealerTotal > 21 && aceCounter > 0) {
           dealerTotal = dealerTotal - 10;
           aceCounter--;
       }
   }
    dealerHand[10]= dealerTotal;
}//end of CheckDealerTotal


//resets the hand
void resetHand() {
    for (int x = 0; x < 10; x++) {
        hand[x] = 0;
        }//end for
    for (int x = 0; x < 10; x++) {
        hand2[x] = 0;
        }//end for
    for (int x = 0; x < 10; x++) {
        hand3[x] = 0;
        }//end for
    for (int x = 0; x < 10; x++) {
        hand4[x] = 0;
        }//end for
    for (int x = 0; x < 10; x++) {
       dealerHand[x] = 0;
       }//end for
}//end of resetHand

//TODO fix issue with the constant loop of you win/ you lose. basically needs a way to cap of the function
void play_dealer() {
    CheckDealerTotal();
    switch(playerCount){
        case 1:
            printf("dealer: %d, Player1: %d\n",dealerHand[10], hand[10] );
            break;
        case 2:
            printf("dealer: %d, Player1: %d, Player2: %d\n", dealerHand[10], hand[10], hand2[10]);
            break;
        case 3:
            printf("dealer: %d, Player1: %d, Player2: %d, Player3: %d\n", dealerHand[10], hand[10], hand2[10], hand3[10]);
            break;
        case 4:
            printf("dealer: %d, Player1: %d, Player2: %d, Player3: %d, Player4: %d\n", dealerHand[10], hand[10], hand2[10], hand3[10], hand4[10]);
            break;

    }
    if (dealerHand[10]>21 || hand[10]==21 || hand2[10]==21 || hand3[10]==21 || hand4[10]==21) {
        printf("YOU WIN\n");
    }
    if(hand[10]>21 || hand2[10]>21 || hand3[10]>21 || hand4[10]>21 || dealerHand[10]==21){
          printf("YOU LOSE\n");
         }
    if (dealerHand[10]<hand[10] || dealerHand<hand2[10] || dealerHand<hand3[10] || dealerHand<hand4[10]){
        dealerHit();
        play_dealer();
    }
}//end of play_dealer


void prompt(int player) {
    char response[20];
    printf("Player %d, hit or stand?\n", player);
    scanf("%s", response);
    if (strcmp(response, "hit") == 0) {
        hit(player);
        print_hand(player);
    }
    else if (strcmp(response, "stand") == 0) {
        player=false;
        falseCount++;
    }
    else {
        printf("ya silly goose that wasn't an option\n");
        prompt(player);
    }


}//end of prompt

void readyGame() {

}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//main
int main(void) {

    shuffle();
    ready_play_deck();
    printf("how many players will be joining us today?\n");
    scanf("%d", &playerCount);
    for (int x=0; x<playerCount+1; x++){
        hit(x);
        hit(x);
    }
    dealerHit();
    dealerHit();
    for (int x=0; x<playerCount+1; x++){
        print_hand(x);
    }
    print_dealerHand();
    while (falseCount<playerCount){
    if(player1==true){
        prompt(1);
    }
    if(player2==true && playerCount>=2){
        prompt(2);
    }
    if(player3==true && playerCount>=3){
        prompt(3);
    }
    if(player4==true && playerCount>=4){
        prompt(4);
    }
    }
    if(playerCount>=1){
    checkHandTotal(hand);
    }
    if(playerCount>=2){
    checkHandTotal(hand2);
    }
    if(playerCount>=3){
        checkHandTotal(hand3);
    }
    if(playerCount>=4){
        checkHandTotal(hand4);
    }
    play_dealer();





  return 0;
}//end of main


