#ifndef CHESS_BOARD
#define CHESS_BOARD

#include <cstdint> 

class ChessBoard {
    private:
        uint64_t white_pawns;
        uint64_t white_knights;
        uint64_t white_bishops;
        uint64_t white_rooks;
        uint64_t white_queens;
        uint64_t white_king;

        uint64_t black_pawns;
        uint64_t black_knights;
        uint64_t black_bishops;
        uint64_t black_rooks;
        uint64_t black_queens;
        uint64_t black_king;

        uint64_t occupied;
        uint64_t white_occupied;
        uint64_t black_occupied;

        bool white_to_move; 
        bool white_king_side_castle;
        bool white_queen_side_castle;
        bool black_king_side_castle;
        bool black_queen_side_castle;

    public:
        ChessBoard();
        void printBitboard(uint64_t board);
        void visualizeBoard();

};

#endif