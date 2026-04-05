#ifndef CARD_H
#define CARD_H

#include <stddef.h>
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

// Status of hand
typedef struct {
    Hand hand;
    uint8_t rank[3];    // Two pair + sentinel
    uint8_t kicker[6];  // No pair + sentinel
} Status;

// Prepare the deck
void prepare_deck(void);

// Reset the deck
void reset_deck(void);

// Discard the deck
void discard_deck(void);

// Shuffle cards in a deck n times
void shuffle_deck(const uint32_t n);

// Draw a card from a top of the deck
Card *draw_from_deck(void);

// Draw a hand (5 cards) from a top of the deck
Card *draw_hand(void);

// Sort cards
Card *sort_cards(Card *cards);

// Get status
void get_status(Status *status, const Card *hand);

// Get suit and number of the card
void get_card_str(char *buf, const size_t size, const Card *card);

// Print hand
void print_hand(Card *hand);

// Print status
void print_status(const Status *status);

#endif  // CARD_H
