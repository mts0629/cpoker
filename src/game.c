#include "game.h"

#include <ctype.h>
#include <errno.h>
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
    static int indices[6] = {0};

    bool has_invalid_token;
    do {
        printf("> Change cards? (index: [0-4]): ");
        char *input = get_input();

        // Clear indices
        for (int i = 0; i < 6; i++) {
            indices[i] = -1;
        }

        const char *p = input;
        int i = 0, j = 0;
        has_invalid_token = false;
        char token[16] = {0};
        while (true) {
            // Separator: space, comma, NUL
            if (isspace(*p) || (*p == ',') || (*p == '\0')) {
                token[j] = '\0';

                // Break when no input
                if (strlen(token) == 0) {
                    break;
                }

                // Parse a token
                errno = 0;
                char *e;
                int n = strtol(token, &e, 10);
                if ((*e != '\0') || (e == token) || (errno == ERANGE)) {
                    printf("> Invalid input: %s\n", token);
                    has_invalid_token = true;
                    break;
                } else {
                    if ((n < 0) || (n > 4)) {
                        printf("> Invalid index: %d\n", n);
                        has_invalid_token = true;
                        break;
                    } else {
                        indices[i] = n;
                        i++;
                    }
                }

                j = 0;
            } else {
                token[j] = *p;
                j++;
            }

            if (i == 5) {
                break;
            }

            if (*p == '\0') {
                break;
            }

            p++;
        }
    } while (has_invalid_token);

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

static void loop_hand(Card *hand) {
    Card *cur = hand;
    char buf[8];
    while (1) {
        get_card_str(buf, sizeof(buf), cur);
        printf("|%s|", buf);
        if (cur->next == NULL) {
            break;
        }

        printf(" ");
        cur = cur->next;
    }
}

void print_hand(Card *hand) {
    printf("          Your hand          \n");
    printf("┌───┐ ┌───┐ ┌───┐ ┌───┐ ┌───┐\n");
    loop_hand(hand);
    printf("\n");
    printf("└───┘ └───┘ └───┘ └───┘ └───┘\n");
}

static void print_hands(Card *you, Card *com) {
    printf("          Your hand           |            COM hand          \n");
    printf("┌───┐ ┌───┐ ┌───┐ ┌───┐ ┌───┐ | ┌───┐ ┌───┐ ┌───┐ ┌───┐ ┌───┐\n");
    loop_hand(you);
    printf(" | ");
    loop_hand(com);
    printf("\n");
    printf("└───┘ └───┘ └───┘ └───┘ └───┘ | └───┘ └───┘ └───┘ └───┘ └───┘\n");
}

void showdown(Player *you, Player *com) {
    printf("> Showdown\n");

    print_hands(you->hand, com->hand);

    get_status(&com->status, com->hand);

    char buf[64];
    get_status_str(buf, &you->status);
    printf("%s", buf);

    printf("\r\033[30C| ");

    get_status_str(buf, &com->status);
    printf("%s", buf);
    printf("\n");

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
