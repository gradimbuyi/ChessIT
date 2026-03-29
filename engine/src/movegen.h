#ifndef MOVEGEN_H
#define MOVEGEN_H

#include "bitboard.h"
#include "movegen.h"
#include "move.h"

#include <vector>

class MoveGenerator {
    public:
        void generateMoves(Bitboard &bitboard, std::vector<Move> &moves);
    
    private:
        void pawnMoves(const Bitboard &bitboard, std::vector<Move> &moves);
        void knightMoves(const Bitboard &bitboard, std::vector<Move> &moves);
        void bishopMoves(const Bitboard &bitboard, std::vector<Move> &moves);
        void rookMoves(const Bitboard &bitboard, std::vector<Move> &moves);
        void queenMoves(const Bitboard &bitboard, std::vector<Move> &moves);
        void kingMoves(const Bitboard &bitboardd, std::vector<Move> &moves);
};

#endif