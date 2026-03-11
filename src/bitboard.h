#ifndef BITBOARD_H
#define BITBOARD_H

#include <cstdint> 

enum bbtype { KING, QUEENS, ROOKS, BISHOPS, KNIGHTS, PAWNS};
enum colors { WHITE, BLACK };

struct BoardState {
    uint64_t pieces[2][6];
    bool side;
};

class Bitboard {
    private:
        uint64_t piecesBB[2][6];
        uint64_t occupied;
        uint64_t white_occupied;
        uint64_t black_occupied;
        bool side; 
        bool castled[2][2]; 
        std::vector<BoardState> history;

        int getPieceType(int color, int square);
        void saveCurrentState();
        void updateOccupancy();

    public:
        Bitboard();
        void printBitboard(const int color, const int type);
        void visualizeBoard();
        uint64_t getPieces(const int color, const int type) const { return piecesBB[color][type]; }
        uint64_t getOccupied() const { return occupied; }
        uint64_t getWhiteOccupied() const { return white_occupied; }
        uint64_t getBlackOccupied() const { return black_occupied; }
        void setPieces(const int color, const int type, uint64_t pieces) { piecesBB[color][type] = pieces; }
        void setMovingSide(bool value) { side = value; }
        bool getMovingSide() const { return side; }
        bool isCastled(const int color, const int type) const { return castled[color][type]; }
        void makeMove(const Move &move);
        void undoMove(const Move &move);
};

#endif