#include "card.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Top of a deck
static Card *top = NULL;

#define NUM_CARDS 4 * 13 + 2

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

    // Add jokers
    for (int i = 0; i < 2; i++) {
        Card *card = malloc(sizeof(Card));
        card->suit = JOKER;
        card->number = 0;
        card->prev = prev;
        card->next = NULL;

        prev->next = card;

        prev = card;

        card_ptrs[idx] = card;
        idx++;
    }
}

void shuffle_deck(uint32_t n) {
    if (top == NULL) {
        return;
    }

    srand(time(NULL));

    uint32_t i = 0;
    while (i < n) {
        int idx = rand() % NUM_CARDS;

        Card *card = top;
        // Get idx-th card from the deck
        for (int j = 0; j < idx; j++) {
            card = card->next;
        }

        // Remove card temporally from the deck
        card->prev->next = card->next;
        card->next->prev = card->prev;

        // Move i-th card to the top
        card->prev = NULL;
        card->next = top;
        top->prev = card;
        top = card;

        i++;
    }
}

void sort_cards(Card *cards) {
    Card *top = cards;
    Card *tmp = top->next;

    while (top != NULL) {
        uint8_t top_n = ((uint8_t)top->suit << 8) | top->number;
        uint8_t tmp_n = ((uint8_t)tmp->suit << 8) | tmp->number;

        if (tmp_n < top_n) {
            tmp->prev->next = tmp->next;
            if (tmp->next != NULL) {
                tmp->next->prev = tmp->prev;
            }

            tmp->next = top;

            if (top->prev != NULL) {
                tmp->prev = top->prev;
                top->prev->next = tmp;
            } else {
                tmp->prev = NULL;
            }

            top->prev = tmp;

            top = tmp;
            tmp = top->next;

            continue;
        }

        if (tmp->next == NULL) {
            if (top->next != NULL) {
                top = top->next;
                tmp = top->next;
                continue;
            } else {
                break;
            }
        }

        tmp = tmp->next;
    }

    if (top->prev == NULL) {
        cards = top;
    } else {
        Card *new_top = top;
        while (1) {
            if (new_top->prev != NULL) {
                new_top = new_top->prev;
            } else {
                break;
            }
        }
    }
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
        case JOKER:
            return "JOK";
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
    snprintf(buf, sizeof(buf), "%s%c%s", get_suit_str(card),
             ((card->suit == JOKER) ? '\0' : ':'),
             ((card->suit == JOKER) ? '\0' : get_num_str(card)));

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

void fini_deck(void) {
    for (int i = 0; i < NUM_CARDS; i++) {
        free(card_ptrs[i]);
    }
}
