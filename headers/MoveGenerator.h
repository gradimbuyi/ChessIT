#ifndef MOVE_GENERATOR
#define MOVE_GENERATOR

#include "../headers/Board.h"
#include "../headers/MoveGenerator.h"
#include <vector>

namespace MoveGenerator {

    std::vector<Move> generateMoves(const ChessBoard &board);

    void generatePawnMoves(const ChessBoard &board, std::vector<Move> &moves);
    void generateKnightMoves(const ChessBoard &board, std::vector<Move> &moves);
    void generateBishopMoves(const ChessBoard &board, std::vector<Move> &moves);
    void generateRookMoves(const ChessBoard &board, std::vector<Move> &moves);
    void generateQueenMoves(const ChessBoard &board, std::vector<Move> &moves);
    void generateKingMoves(const ChessBoard &board, std::vector<Move> &moves);
    void printMoves(const std::vector<Move> &moves);
    bool isInCheck(const ChessBoard &board, bool white);
    bool isSquareAttacked(const ChessBoard &board, int square, bool by_white);
}

#endif