#include "bitboard.h"
#include "console.h"

int main() {
    Bitboard bitboard;
    Console console(bitboard);

    console.runEngineCLI();

    return 0;
}