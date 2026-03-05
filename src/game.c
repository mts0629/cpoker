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

    Hand h = NO_PAIR;
    Card *c = sorted;
    while (c != NULL) {
        count.suit_count[c->suit]++;
        count.num_count[c->number]++;
        c = c->next;
    }

    int count_pair = 0;
    for (int i = 1; i <= 13; i++) {
        if (count.num_count[i] == 2) {
            count_pair++;
        }
    }

    if (count_pair == 1) {
        h = ONE_PAIR;
    } else if (count_pair == 2) {
        h = TWO_PAIR;
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
