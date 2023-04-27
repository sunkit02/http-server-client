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
#include "displayCards.h"


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
Card popCard(CardArrayWrapper *arrayWrapper) {
    if (arrayWrapper->size) {}//TODO: make sure the stack isn't empty. If it is, exit program.

}

#include "displayCards.h"
void convertAndPrint(char *data[], size_t length) {
    CardArrayWrapper cardArrayWrapper = constructCardArray(length);
    for (size_t i = 0; i < length; i++) {
        Card card = { data[i][0], data[i][1] };
        addToCardArray(&cardArrayWrapper, card);
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
