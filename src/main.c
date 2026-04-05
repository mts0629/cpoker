#include <stdbool.h>
#include <stdio.h>

#include "game.h"

int main(void) {
    prepare_deck();

    Player you, com;
    bool playing = true;
    while (playing) {
        reset_deck();

        shuffle_deck(100);

        you.hand = draw_hand();
        you.hand = sort_cards(you.hand);

        com.hand = draw_hand();
        com.hand = sort_cards(com.hand);

        get_status(&you.status, you.hand);

        print_hand(you.hand);
        print_status(&you.status);

        int *indices = parse_input();
        you.hand = change_cards(you.hand, indices, true);
        you.hand = sort_cards(you.hand);
        get_status(&you.status, you.hand);

        indices = com_think();
        com.hand = change_cards(com.hand, indices, false);
        com.hand = sort_cards(com.hand);

        if (check_call()) {
            showdown(&you, &com);
        } else {
            printf("> You folded...\n");
        }

        playing = check_contine();
    }

    discard_deck();

    return 0;
}
