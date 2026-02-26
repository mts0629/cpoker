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

int main(void) {
    // Init a deck
    Card deck[4 * 13 + 2];
    for (uint8_t i = 0; i < 4; i++) {
        uint8_t s = i << 4;
        for (uint8_t j = 0; j < 13; j++) {
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
        // Print suit
        switch (cards[i] & 0xf0) {
            case 0x00:
                printf("H");
                break;
            case 0x10:
                printf("D");
                break;
            case 0x20:
                printf("C");
                break;
            case 0x30:
                printf("S");
                break;
            case 0x40:
                printf("JOK ");
                continue;
                break;
            default:
                fprintf(stderr, "Invalid card\n");
                exit(1);
                break;
        }
        int n = cards[i] & 0x0f;
        switch (n) {
            case 12:
                printf(":K ");
                break;
            case 11:
                printf(":Q ");
                break;
            case 10:
                printf(":J ");
                break;
            case 0:
                printf(":A ");
                break;
            default:
                printf(":%d ", (n + 1));
                break;
        }
    }
    printf("\n");

    return 0;
}
