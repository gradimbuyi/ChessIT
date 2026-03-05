#ifndef MOVEGEN_H
#define MOVEGEN_H

#include "bitboard.h"
#include "movegen.h"
#include "move.h"

#include <vector>

namespace MoveGenerator {

    std::vector<Move> generate_moves(const Bitboard &bitboard);

    void pawnMoves(const Bitboard &bitboard, std::vector<Move> &moves);
    void knightMoves(const Bitboard &bitboard, std::vector<Move> &moves);
    void bishopMoves(const Bitboard &bitboard, std::vector<Move> &moves);
    void rookMoves(const Bitboard &bitboard, std::vector<Move> &moves);
    void queenMoves(const Bitboard &bitboard, std::vector<Move> &moves);
    void kingMoves(const Bitboard &bitboardd, std::vector<Move> &moves);
}

#endif