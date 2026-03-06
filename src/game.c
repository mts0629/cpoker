#include <stdio.h>

#include "card.h"

// Hand in pokers
typedef enum {
    NO_PAIR,
    ONE_PAIR,
    TWO_PAIR,
    THREE_OF_A_KIND,
    STRAIGHT,
    FLUSH,
    FULL_HOUSE,
    FOUR_OF_A_KIND,
    STRAIGHT_FLUSH,
    ROYAL_FLUSH,
    FIVE_OF_A_KIND,
} Hand;

typedef struct {
    int suit_count[5];
    int num_count[14];
} Count;

Hand get_hand(Card *hand) {
    Card *sorted = sort_cards(hand);

    Count count = {
        .suit_count = {0},
        .num_count = {0},
    };

    Card *c = sorted;
    uint8_t min_num = UINT8_MAX;
    uint8_t max_num = 0;
    while (c != NULL) {
        count.suit_count[c->suit]++;
        count.num_count[c->number]++;

        if (c->number < min_num) {
            min_num = c->number;
        }
        if (c->number > max_num) {
            max_num = c->number;
        }

        c = c->next;
    }

    Hand h = NO_PAIR;
    int count_pairs = 0;
    int num_series = 1;
    for (int i = 1; i <= 13; i++) {
        if (count.num_count[i] == 2) {
            count_pairs++;
        }
        if (count.num_count[i] == 3) {
            h = THREE_OF_A_KIND;
        }
        if (count.num_count[i] == 4) {
            h = FOUR_OF_A_KIND;
        }
        if ((count.num_count[i - 1] == 1) && (count.num_count[i] == 1)) {
            num_series++;
        }
    }

    if (count_pairs == 1) {
        if (h == THREE_OF_A_KIND) {
            h = FULL_HOUSE;
        } else {
            h = ONE_PAIR;
        }
    } else if (count_pairs == 2) {
        h = TWO_PAIR;
    }

    if ((num_series == 5) ||
        ((num_series == 4) && (min_num == 1) && (max_num == 13))) {
        h = STRAIGHT;
    }

    for (int i = 0; i < 4; i++) {
        if (count.suit_count[i] == 5) {
            if (h == STRAIGHT) {
                if ((min_num == 1) && (max_num == 13)) {
                    h = ROYAL_FLUSH;
                } else {
                    h = STRAIGHT_FLUSH;
                }
            } else {
                h = FLUSH;
            }
        }
    }

    return h;
}

char *get_hand_str(const Hand hand) {
    switch (hand) {
        case NO_PAIR:
            return "No pair";
            break;
        case ONE_PAIR:
            return "One pair";
            break;
        case TWO_PAIR:
            return "Two pair";
            break;
            break;
        case THREE_OF_A_KIND:
            return "Three of a kind";
            break;
        case STRAIGHT:
            return "Straight";
            break;
        case FLUSH:
            return "Flush";
            break;
        case FULL_HOUSE:
            return "Full house";
            break;
        case FOUR_OF_A_KIND:
            return "Four of a kind";
            break;
        case STRAIGHT_FLUSH:
            return "Straight flush";
            break;
        case ROYAL_FLUSH:
            return "Royal flush";
            break;
        case FIVE_OF_A_KIND:
            return "Five of a kind";
            break;
        default:
            fprintf(stderr, "Invalid hand\n");
            return NULL;
            break;
    }
}

int main(void) {
    init_deck();

    shuffle_deck(100);

    Card *hand = draw_hand();

    hand = sort_cards(hand);
    print_cards(hand);

    printf("%s\n", get_hand_str(get_hand(hand)));

    fini_deck();

    return 0;
}
