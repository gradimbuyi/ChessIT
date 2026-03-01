#ifndef BITBOARD_H
#define BITBOARD_H

#include <cstdint> 

/*
#define A_FILE 0x0101010101010101
#define H_FILE 0x8080808080808080
#define FIRST_RANK 0x00000000000000FF
#define SECOND_RANK 0x000000000000FF00ULL
#define SEVENTH_RANK 0x00FF000000000000ULL
#define EIGHTH_RANK 0xFF00000000000000
#define A1_H8_DIAGONAL 0x8040201008040201
#define H1_A8_ANTIDIAGONAL 0x0102040810204080
#define LIGHT_SQUARES 0x55AA55AA55AA55AA
#define DARK_SQUARES 0xAA55AA55AA55AA55
enum squares {
    a1, b1, c1, d1, e1, f1, g1, h1,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a8, b8, c8, d8, e8, f8, g8, h8
};


*/
enum bbtype { KING, QUEENS, ROOKS, BISHOPS, KNIGHTS, PAWNS};
enum colors { WHITE, BLACK };

class Bitboard {
    private:
        uint64_t piecesBB[2][6];
        uint64_t occupied;
        uint64_t white_occupied;
        uint64_t black_occupied;
        bool side; 
        bool castled[2][2]; 

    public:
        Bitboard();
        void printBitboard(const int color, const int type);
        void visualizeBoard();
        uint64_t getPieces(const int color, const int type) const { return piecesBB[color][type]; }
        uint64_t getOccupied() const { return occupied; }
        uint64_t getWhiteOccupied() const { return white_occupied; }
        uint64_t getBlackOccupied() const { return black_occupied; }
        bool getMovingSide() const { return side; }
        bool isCastled(const int color, const int type) const { return castled[color][type]; }
};

#endif