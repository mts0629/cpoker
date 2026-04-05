#include "game.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *get_input(void) {
    static char input[32];
    memset(input, (int)'\0', sizeof(input));

    fgets(input, sizeof(input), stdin);

    size_t len = strlen(input);
    if (input[len - 1] != '\n') {
        // Skip remaining chars
        while (getchar() != '\n')
            ;
    }

    input[len - 1] = '\0';

    return input;
}

int *parse_input(void) {
    printf("> Change indices [0-4]: ");

    char *input = get_input();

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
                fprintf(stderr, "> Error index: %d\n", d);
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

static bool command_is(char *str, const char *command) {
    // Convert string to small letters
    char *p = str;
    while (*p != '\0') {
        *p = tolower(*p);
        p++;
    }

    if (strcmp(str, command) == 0) {
        return true;
    }

    return false;
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

Card *change_cards(Card *hand, const int *indices, const bool is_player) {
    if (indices[0] == -1) {
        if (is_player) {
            printf("> Player's cards didn't be changed\n");
        } else {
            printf("> COM's cards didn't be changed\n");
        }
        return hand;
    }

    Card *head = hand;

    if (is_player) {
        printf("> Change cards: ");
    }

    int i = 0;
    while (indices[i] != -1) {
        Card *new = draw_from_deck();

        Card *old = head;
        for (int j = 0; j < indices[i]; j++) {
            old = old->next;
        }

        if (is_player) {
            char buf[16], buf2[16];
            get_card_str(buf, sizeof(buf), old);
            get_card_str(buf2, sizeof(buf2), new);
            printf("%s to %s", buf, buf2);
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

        if (is_player) {
            if (indices[i] != -1) {
                printf(", ");
            }
        }
    }

    if (is_player) {
        printf("\n");
    } else {
        printf("> COM changed %d cards\n", i);
    }

    return head;
}

bool check_call(void) {
    printf("> Call or fold? [c/f]: ");

    while (true) {
        char *input = get_input();

        if (command_is(input, "c")) {
            return true;
        } else if (command_is(input, "f")) {
            break;
        }

        printf("> Plase input 'c' (call) or 'f' (fold): ");
    }

    return false;
}

static void judge(const Status *your_status, const Status *com_status) {
    // Compare hand
    if (your_status->hand > com_status->hand) {
        printf("> You win!\n");
    } else if (your_status->hand < com_status->hand) {
        printf("> You lose...\n");
    } else {
        // Compare rank
        if (your_status->rank[0] > com_status->rank[0]) {
            printf("> You win!\n");
        } else if (your_status->rank[0] < com_status->rank[0]) {
            printf("> You lose...\n");
        } else if (your_status->hand == TWO_PAIR) {
            // Compare 2nd rank
            if (your_status->rank[1] > com_status->rank[1]) {
                printf("> You win!\n");
            } else if (your_status->rank[1] < com_status->rank[1]) {
                printf("> You lose...\n");
            }
        } else {
            // Compare kickers
            int i = 0;
            while (your_status->kicker[i] != 0) {
                if (your_status->kicker[i] > com_status->kicker[i]) {
                    printf("> You win!\n");
                    break;
                } else if (your_status->kicker[i] < com_status->kicker[i]) {
                    printf("> You lose...\n");
                    break;
                }
                i++;
            }

            if (your_status->kicker[i] == 0) {
                printf("> Draw\n");
            }
        }
    }
}

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

bool check_contine(void) {
    printf("> Play again? [y/n]: ");

    while (true) {
        char *input = get_input();

        if (command_is(input, "y")) {
            return true;
        } else if (command_is(input, "n")) {
            break;
        }

        printf("> Plase input 'y' (yes) or 'n' (no): ");
    }

    return false;
}
