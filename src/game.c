#include <stdio.h>

#include "card.h"

void judge(const Status *player, const Status *com) {
    // Compare hand
    if (player->hand > com->hand) {
        printf("You win!\n");
        return;
    } else if (player->hand < com->hand) {
        printf("You lose...\n");
        return;
    } else {
        // Compare rank
        if (player->rank[0] > com->rank[0]) {
            printf("You win!\n");
            return;
        } else if (player->rank[0] < com->rank[0]) {
            printf("You lose...\n");
            return;
        } else {
            // Compare 2nd rank
            if (player->hand == TWO_PAIR) {
                if (player->rank[1] > com->rank[1]) {
                    printf("You win!\n");
                    return;
                } else if (player->rank[1] < com->rank[1]) {
                    printf("You lose...\n");
                    return;
                }
            }

            // Compare kickers
            int i = 0;
            while (player->kicker[i] != 0) {
                if (player->kicker[i] > com->kicker[i]) {
                    printf("You win!\n");
                    return;
                } else if (player->kicker[i] < com->kicker[i]) {
                    printf("You lose...\n");
                    return;
                }
                i++;
            }
        }

        printf("Draw\n");
    }
}

int main(void) {
    init_deck();

    shuffle_deck(100);

    Card *player_hand = draw_hand();
    player_hand = sort_cards(player_hand);

    Card *com_hand = draw_hand();
    com_hand = sort_cards(com_hand);

    Status player_status;
    get_status(&player_status, player_hand);

    printf("--------- Your card ---------\n");
    print_cards(player_hand);
    print_status(&player_status);

    Status com_status;
    get_status(&com_status, com_hand);

    printf("---------- COM card ---------\n");
    print_cards(com_hand);
    print_status(&com_status);
    printf("-----------------------------\n");

    judge(&player_status, &com_status);

    fini_deck();

    return 0;
}
