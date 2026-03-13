#include "card.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Top of a deck
static Card *top = NULL;

#define NUM_CARDS 4 * 13

// List of pointers of cards (used for deallocation)
static Card *card_ptrs[NUM_CARDS];

void init_deck(void) {
    Card *prev = top;

    // Add cards to the deck
    int idx = 0;
    for (int s = 0; s < 4; s++) {
        for (uint8_t n = 1; n <= 13; n++) {
            Card *card = malloc(sizeof(Card));
            card->suit = (Suit)s;
            card->number = n;
            card->prev = prev;
            card->next = NULL;

            if (top == NULL) {
                top = card;
            } else {
                prev->next = card;
            }

            prev = card;

            card_ptrs[idx] = card;
            idx++;
        }
    }
}

void shuffle_deck(uint32_t n) {
    if (top == NULL) {
        return;
    }

    srand(time(NULL));

    uint32_t i = 0;
    while (i < n) {
        Card *card = top;

        // Get idx-th card from the deck
        int idx = rand() % NUM_CARDS;
        for (int j = 0; j <= idx; j++) {  // 0 to 51
            card = card->next;
        }

        // Remove card temporally from the deck
        if (card->prev != NULL) {  // Not the top
            card->prev->next = card->next;
        }
        if (card->next != NULL) {  // Not the last
            card->next->prev = card->prev;
        }

        // Move idx-th card to the top
        card->prev = NULL;
        card->next = top;
        top->prev = card;
        top = card;

        i++;
    }
}

Card *sort_cards(Card *cards) {
    Card *unsorted_head = cards;      // Head of unsorted part in the cards
    Card *cmp = unsorted_head->next;  // Card to be compared

    while (1) {
        uint8_t unsorted_head_n =
            ((uint8_t)unsorted_head->suit << 4) | unsorted_head->number;
        uint8_t cmp_n = ((uint8_t)cmp->suit << 4) | cmp->number;

        if (cmp_n < unsorted_head_n) {
            // If cmp is smaller than unsorted_head, move cmp to the
            // unsorted_head
            cmp->prev->next = cmp->next;
            if (cmp->next != NULL) {
                cmp->next->prev = cmp->prev;
            }
            cmp->next = unsorted_head;
            if (unsorted_head->prev != NULL) {
                // If unsorted_head has previous cards, insert cmp
                unsorted_head->prev->next = cmp;
            }
            cmp->prev = unsorted_head->prev;
            unsorted_head->prev = cmp;

            // And restart comparison from unsorted_head
            unsorted_head = cmp;
            cmp = unsorted_head->next;

            continue;
        }

        // If cmp is the last card, move unsorted_head to its next card
        if (cmp->next == NULL) {
            unsorted_head = unsorted_head->next;
            // If unsorted_head is the last card, finish
            if (unsorted_head->next == NULL) {
                break;
            }
            cmp = unsorted_head->next;
            continue;
        }

        cmp = cmp->next;
    }

    // Update the head of the cards
    Card *new_head = unsorted_head;
    while (new_head->prev != NULL) {
        new_head = new_head->prev;
    }

    return new_head;
}

static char *get_suit_str(const Card *card) {
    switch (card->suit) {
        case HEART:
            return "H";
            break;
        case DIAMOND:
            return "D";
            break;
        case CLUB:
            return "C";
            break;
        case SPADE:
            return "S";
            break;
        default:
            fprintf(stderr, "Invalid suit\n");
            return NULL;
            break;
    }
}

static char *get_num_str(const Card *card) {
    if (card->number > 13) {
        fprintf(stderr, "Invalid number\n");
        return NULL;
    }

    static char buf[3];
    switch (card->number) {
        case 13:
            return "K";
            break;
        case 12:
            return "Q";
            break;
        case 11:
            return "J";
            break;
        case 1:
            return "A";
            break;
        default:
            snprintf(buf, sizeof(buf), "%d", card->number);
            return buf;
            break;
    }
}

static char *get_card_str(const Card *card) {
    static char buf[5];
    snprintf(buf, sizeof(buf), "%s:%s", get_suit_str(card), get_num_str(card));

    return buf;
}

void print_cards(Card *hand) {
    Card *cur = hand;
    while (1) {
        printf("%s", get_card_str(cur));
        if (cur->next == NULL) {
            break;
        }

        printf(" ");
        cur = cur->next;
    }
    printf("\n");
}

Card *draw_from_deck(void) {
    if (top == NULL) {
        return NULL;
    }

    Card *drawn = top;

    top = top->next;
    if (top != NULL) {
        top->prev = NULL;
    }

    drawn->next = NULL;

    return drawn;
}

Card *draw_hand(void) {
    Card *head = NULL;
    Card *prev = head;

    for (int i = 0; i < 5; i++) {
        Card *card = draw_from_deck();
        if (head == NULL) {
            head = card;
        } else {
            prev->next = card;
            card->prev = prev;
        }

        prev = card;
    }

    return head;
}

// Card count
typedef struct {
    int suit[4];
    int number[14];
} Count;

static Hand check_hand(const Card *hand, uint8_t *rank) {
    Count count = {
        .suit = {0},
        .number = {0},
    };

    Card *card = (Card *)hand;
    uint8_t min_num = UINT8_MAX;
    uint8_t max_num = 0;
    while (card != NULL) {
        count.suit[card->suit]++;
        count.number[card->number]++;

        if (card->number < min_num) {
            min_num = card->number;
        }
        if (card->number > max_num) {
            max_num = card->number;
        }

        card = card->next;
    }

    Hand type = NO_PAIR;
    uint8_t *p_rank = rank;
    int count_pairs = 0;
    int num_seq = 1;
    for (uint8_t i = 1; i <= 13; i++) {
        if (count.number[i] == 2) {
            count_pairs++;
            if (i > *p_rank) {
                *p_rank = i;
            }
            p_rank++;
        } else if (count.number[i] == 3) {
            type = THREE_OF_A_KIND;
            *p_rank = i;
            p_rank++;
        } else if (count.number[i] == 4) {
            type = FOUR_OF_A_KIND;
            *p_rank = i;
        }

        if ((count.number[i - 1] == 1) && (count.number[i] == 1)) {
            num_seq++;
        }
    }

    if (count_pairs == 1) {
        if (type == THREE_OF_A_KIND) {
            type = FULL_HOUSE;
        } else {
            type = ONE_PAIR;
        }
    } else if (count_pairs == 2) {
        type = TWO_PAIR;
    }

    if ((num_seq == 5) || ((count.number[1] == 1) && (count.number[10] == 1) &&
                           (count.number[11] == 1) && (count.number[12] == 1) &&
                           (count.number[13] == 1))) {
        type = STRAIGHT;
        *p_rank = max_num;
    }

    for (int i = 0; i < 4; i++) {
        if (count.suit[i] == 5) {
            if (type == STRAIGHT) {
                if ((count.number[1] == 1) && (count.number[10] == 1) &&
                    (count.number[11] == 1) && (count.number[12] == 1) &&
                    (count.number[13] == 1)) {
                    type = ROYAL_FLUSH;
                    *p_rank = 14;
                } else {
                    type = STRAIGHT_FLUSH;
                    *p_rank = max_num;
                }
            } else {
                type = FLUSH;
                *p_rank = max_num;
            }
        }
    }

    return type;
}

static char *get_hand_str(const Hand hand) {
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
        default:
            fprintf(stderr, "Invalid hand\n");
            return NULL;
            break;
    }
}

void print_hand(const Card *hand) {
    uint8_t rank[5] = {0};
    Hand type = check_hand(hand, rank);

    printf("%s", get_hand_str(type));
    if ((type != NO_PAIR) || (type != ROYAL_FLUSH)) {
        uint8_t *p_rank = rank;
        printf(" (rank: ");
        while (1) {
            printf("%u", *p_rank);
            p_rank++;
            if (*p_rank != 0) {
                printf(",");
            } else {
                break;
            }
        }
        printf(")");
    }
    printf("\n");
}

void fini_deck(void) {
    for (int i = 0; i < NUM_CARDS; i++) {
        free(card_ptrs[i]);
    }
}
