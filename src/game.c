#include <ctype.h>
#include <stdbool.h>
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

int *com_think(void) {
    static int indices[6] = {0};
    for (int i = 0; i < 6; i++) {
        indices[i] = -1;
    }

    // Just change random indices
    int ns[] = {0, 1, 2, 3, 4, -1};
    for (int i = 0; i < 5; i++) {
        int n = rand() % 6;
        if (ns[n] == -1) {
            break;
        }

        indices[i] = ns[n];

        ns[n] = -1;
    }

    return indices;
}

Card *change_cards(Card *hand, const int *indices, const bool print_card) {
    if (indices[0] == -1) {
        printf("> Cards didn't be changed\n");
        return hand;
    }

    Card *head = hand;

    int i = 0;
    while (indices[i] != -1) {
        Card *new = draw_from_deck();

        Card *old = head;
        for (int j = 0; j < indices[i]; j++) {
            old = old->next;
        }

        if (print_card) {
            printf("> Change %s with a drawn card: ", get_card_str(old));
            printf("%s\n", get_card_str(new));
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

void judge(const Status *your_status, const Status *com_status) {
    // com_statuspare hand
    printf("=============================\n");
    if (your_status->hand > com_status->hand) {
        printf("          You win!\n");
    } else if (your_status->hand < com_status->hand) {
        printf("         You lose...\n");
    } else {
        // com_statuspare rank
        if (your_status->rank[0] > com_status->rank[0]) {
            printf("          You win!\n");
        } else if (your_status->rank[0] < com_status->rank[0]) {
            printf("         You lose...\n");
        } else if (your_status->hand == TWO_PAIR) {
            // com_statuspare 2nd rank
            if (your_status->rank[1] > com_status->rank[1]) {
                printf("          You win!\n");
            } else if (your_status->rank[1] < com_status->rank[1]) {
                printf("         You lose...\n");
            }
        } else {
            // com_statuspare kickers
            int i = 0;
            while (your_status->kicker[i] != 0) {
                if (your_status->kicker[i] > com_status->kicker[i]) {
                    printf("          You win!\n");
                    break;
                } else if (your_status->kicker[i] < com_status->kicker[i]) {
                    printf("         You lose...\n");
                    break;
                }
                i++;
            }

            if (your_status->kicker[i] == 0) {
                printf("            Draw\n");
            }
        }
    }
    printf("=============================\n");
}

// Player's hand and status
typedef struct {
    Card *hand;
    Status status;
} Player;

void showdown(Player *you, Player *com) {
    printf("=============================\n");
    printf("          Showdown\n");
    printf("=============================\n");

    printf("--------- Your card ---------\n");
    print_hand(you->hand);
    print_status(&you->status);

    printf("---------- COM card ---------\n");
    print_hand(com->hand);
    get_status(&com->status, com->hand);
    print_status(&com->status);

    judge(&you->status, &com->status);
}

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

        printf("--------- Your card ---------\n");
        print_hand(you.hand);
        print_status(&you.status);

        printf("Change indices (0-4): ");
        int *indices = parse_input();

        you.hand = change_cards(you.hand, indices, true);
        you.hand = sort_cards(you.hand);
        get_status(&you.status, you.hand);

        indices = com_think();
        com.hand = change_cards(com.hand, indices, false);
        com.hand = sort_cards(com.hand);

        int i = 0;
        while (indices[i] != -1) {
            i++;
        }
        printf("> COM changed %d cards\n", i);

        showdown(&you, &com);

        printf("Play again? (y/n): ");
        char input[2];
        while (1) {
            fgets(input, sizeof(input), stdin);

            while (getchar() != '\n')
                ;  // Skip remaining chars

            if ((input[0] == 'y') || (input[0] == 'Y')) {
                playing = true;
                break;
            } else if ((input[0] == 'n') || (input[0] == 'N')) {
                playing = false;
                break;
            }

            printf("Input 'y'/'Y'or 'n'/'N': ");
        }
    }

    discard_deck();

    return 0;
}
