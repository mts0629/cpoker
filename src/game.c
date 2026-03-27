#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "card.h"

int *parse_input(void) {
    char input[32];
    fgets(input, sizeof(input), stdin);

    static int indices[6] = {0};
    for (int i = 0; i < 6; i++) {
        indices[i] = -1;
    }

    const char *p = input;
    int i = 0;
    char buf[2] = {0};
    while (*p != '\0') {
        if (isdigit(*p)) {
            buf[0] = *p;

            int d = strtol(buf, NULL, 10);
            if ((d < 0) || (d > 4)) {
                fprintf(stderr, "Error index: %d\n", d);
            } else {
                indices[i] = d;
                i++;
            }
        }

        if (i == 5) {
            break;
        }

        p++;
    }

    return indices;
}

Card *change_cards(Card *player_hand, const int *indices) {
    Card *head = player_hand;

    int i = 0;
    while (indices[i] != -1) {
        Card *new = draw_from_deck();

        Card *old = head;
        for (int j = 0; j < indices[i]; j++) {
            old = old->next;
        }

        if (old->prev != NULL) {
            old->prev->next = new;
        }
        new->prev = old->prev;

        if (old->next != NULL) {
            old->next->prev = new;
        }
        new->next = old->next;

        head = new;
        while (head->prev != NULL) {
            head = head->prev;
        }

        i++;
    }

    return head;
}

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

    reset_deck();

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

    printf("Change indices (0-4): ");
    int *indices = parse_input();

    player_hand = change_cards(player_hand, indices);
    player_hand = sort_cards(player_hand);

    get_status(&player_status, player_hand);

    printf("Showdown\n");

    printf("--------- Your card ---------\n");
    print_cards(player_hand);
    print_status(&player_status);

    printf("---------- COM card ---------\n");
    print_cards(com_hand);
    print_status(&com_status);
    printf("-----------------------------\n");

    judge(&player_status, &com_status);

    fini_deck();

    return 0;
}
