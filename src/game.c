#include <stdio.h>

#include "card.h"

int main(void) {
    init_deck();

    shuffle_deck(100);

    Card *hand = draw_hand();
    print_cards(hand);

    fini_deck();

    return 0;
}
