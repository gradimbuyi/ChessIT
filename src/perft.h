#ifndef PERFT_H
#define PERFT_H

#include <string>

#include "bitboard.h"
#include "movegen.h"

class Perft {
    private:
        MoveGenerator movegen;
        uint64_t perftRecurse(Bitboard &bitboard, int depth);

    public:
        void runPerft(Bitboard &bitboard, int depth, const std::string &filename);
        void runPerftDivide(Bitboard &bitboard, int depth, const std::string &filename);
};

#endif