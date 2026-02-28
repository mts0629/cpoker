#ifndef CARD_H
#define CARD_H

#include <stdint.h>

typedef enum { HEART, DIAMOND, CLUB, SPADE, JOKER, NONE } Suit;

typedef struct Card {
    Suit suit;
    uint8_t number;
    struct Card *prev;
    struct Card *next;
} Card;

void init_deck(void);

char *get_card_str(const Card *card);

Card *draw_from_deck(void);

void release_deck(void);

#endif  // CARD_H
