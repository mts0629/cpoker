#ifndef CARD_H
#define CARD_H

#include <stdint.h>

// Suit
typedef enum { HEART, DIAMOND, CLUB, SPADE, NONE } Suit;

// Card
typedef struct Card {
    Suit suit;
    uint8_t number;
    struct Card *prev;
    struct Card *next;
} Card;

// Hands in poker
typedef enum {
    NO_PAIR,
    ONE_PAIR,
    TWO_PAIR,
    THREE_OF_A_KIND,
    STRAIGHT,
    FLUSH,
    FULL_HOUSE,
    FOUR_OF_A_KIND,
    STRAIGHT_FLUSH,
    ROYAL_FLUSH
} Hand;

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

// Print a hand
void print_hand(const Card *hand);

// Finalize the deck
void fini_deck(void);

#endif  // CARD_H
