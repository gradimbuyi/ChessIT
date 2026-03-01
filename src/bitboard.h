#ifndef BITBOARD_H
#define BITBOARD_H

#include <cstdint> 


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