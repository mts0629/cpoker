#include <stdio.h>

#include "card.h"

int main(void) {
    init_deck();

    // See all cards in a deck
    while (1) {
        Card *card = draw_from_deck();
        if (card == NULL) {
            break;
        }

        printf("%s ", get_card_str(card));
    }
    printf("\n");

    release_deck();

    return 0;
}
