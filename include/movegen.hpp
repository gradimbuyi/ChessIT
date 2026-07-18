#pragma once

#include "bitboard.hpp"
#include "move.hpp"

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
        void kingMoves(const Bitboard &bitboard, std::vector<Move> &moves);
};