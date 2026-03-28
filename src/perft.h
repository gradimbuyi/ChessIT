#ifndef PERFT_H
#define PERFT_H

#include <cstdint>

#include "bitboard.h"
#include "movegen.h"

class Perft {
    private:
        MoveGenerator movegen;
        uint64_t perftRecurse(Bitboard &bitboard, int depth);

    public:
        void runPerft(Bitboard &bitboard, int depth);
        void runPerftDivide(Bitboard &bitboard, int depth);
};

#endif