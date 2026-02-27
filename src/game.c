#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define H 0x00  // Hearts
#define D 0x10  // Diamonds
#define C 0x20  // Clubs
#define S 0x30  // Spades

typedef uint8_t Card;

char *get_suit(const Card *card) {
    switch (*card & 0xf0) {
        case 0x00:
            return "H";
            break;
        case 0x10:
            return "D";
            break;
        case 0x20:
            return "C";
            break;
        case 0x30:
            return "S";
            break;
        case 0x40:
            return "JOK";
            break;
        default:
            fprintf(stderr, "Invalid suit\n");
            return NULL;
            break;
    }
}

char *get_num_str(const Card *card) {
    if (*card == 0x40) {
        return "";
    }

    int n = *card & 0x0f;
    static char buf[3];
    switch (n) {
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
            snprintf(buf, sizeof(buf), "%d", n);
            return buf;
            break;
    }
}

char *get_card_str(const Card *card) {
    static char buf[5];
    snprintf(buf, sizeof(buf), "%s:%s", get_suit(card), get_num_str(card));
    return buf;
}

int main(void) {
    // Init a deck
    Card deck[4 * 13 + 2];
    for (uint8_t i = 0; i < 4; i++) {
        uint8_t s = i << 4;
        for (uint8_t j = 1; j <= 13; j++) {
            deck[i * 13 + j] = s + j;
        }
    }
    // Jokers
    deck[52] = 0x40;
    deck[53] = 0x40;

    // Random pick
    srand((unsigned)time(NULL));
    int selected[4 * 13 + 2] = {0};
    Card cards[5];
    int n = 0;
    while (n < 5) {
        int idx = rand() % (4 * 13 + 2);
        if (selected[idx] == 0) {
            cards[n] = deck[idx];
            selected[idx] = 1;
            n++;
        }
    }

    // Show cards
    for (int i = 0; i < 5; i++) {
        printf("%s ", get_card_str(&cards[i]));
    }
    printf("\n");

    return 0;
}
