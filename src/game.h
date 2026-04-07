#ifndef GAME_H
#define GAME_H

#include <stdbool.h>

#include "card.h"

// Player's hand and status
typedef struct {
    Card *hand;
    Status status;
} Player;

int *parse_input(void);

int *com_think(void);

Card *change_cards(Card *hand, const int *indices, const bool is_player);

bool check_call(void);

bool check_contine(void);

void print_hand(Card *hand);

void showdown(Player *you, Player *com);

#endif  // GAME_H
