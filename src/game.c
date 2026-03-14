#include <stdio.h>

#include "card.h"

int main(void) {
    init_deck();

    shuffle_deck(100);

    Card *hand = draw_hand();
    hand = sort_cards(hand);

    Status status;
    get_status(&status, hand);

    print_cards(hand);
    print_status(&status);

    fini_deck();

    return 0;
}
