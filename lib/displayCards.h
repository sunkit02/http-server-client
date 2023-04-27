#ifndef DISPLAY_CARDS_H
#define DISPLAY_CARDS_H

#include <stdio.h>
#include <stdlib.h>


typedef struct Card {
    char face;
    char suit;
} Card;

typedef struct CardArrayWrapper {
    Card *elementArray;
    size_t size;        // current size
    size_t capacity;    // max size
} CardArrayWrapper;

CardArrayWrapper constructCardArray(size_t initialCapacity);

void addToCardArray(CardArrayWrapper *arrayWrapper, Card card);

// Clears all the cards stored and resets the arrayWrapper
void clearCardsInHand(CardArrayWrapper *arrayWrapper);

//void cardAssign(Card *card, char face, char suit);
void displayCardLine(Card handToRead[], int handToReadLength, int currentLine);
//void displayCards(Card handToRead[], size_t handToReadLength);
void displayCards(CardArrayWrapper hand);

void convertAndPrint(char *data[], size_t length);

#endif
