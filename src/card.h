#ifndef CARD_H
#define CARD_H

#include <stdint.h>

// Suit
typedef enum { HEART, DIAMOND, CLUB, SPADE, JOKER, NONE } Suit;

// Card
typedef struct Card {
    Suit suit;
    uint8_t number;
    struct Card *prev;
    struct Card *next;
} Card;

// Initialize the deck
void init_deck(void);

// Shuffle cards in a deck n times
void shuffle_deck(const uint32_t n);

// Sort cards
Card *sort_cards(Card *cards);

// Print cards
void print_cards(Card *hand);

// Draw a card from a top of the deck
Card *draw_from_deck(void);

// Draw a hand (5 cards) from a top of the deck
Card *draw_hand(void);

// Finalize the deck
void fini_deck(void);

#endif  // CARD_H
