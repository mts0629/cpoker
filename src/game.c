#include <stdio.h>

#include "card.h"

int main(void) {
    init_deck();

    shuffle_deck(100);

    Card *hand = draw_hand();

    hand = sort_cards(hand);
    print_cards(hand);

    printf("%s\n", get_hand_str(check_hand(hand)));

    fini_deck();

    return 0;
}
