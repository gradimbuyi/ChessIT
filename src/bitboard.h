#ifndef BITBOARD_H
#define BITBOARD_H

#include <cstdint> 

#include "move.h"

enum bbtype { KING, QUEENS, ROOKS, BISHOPS, KNIGHTS, PAWNS};
enum colors { WHITE, BLACK };

struct BoardState {
    uint64_t pieces[2][6];
    bool side;
    int ep_square;
    CastlingRight castling;
};

struct CastlingRight {
    bool king_side[2];
    bool queen_side[2];
};

class Bitboard {
    private:
        uint64_t piecesBB[2][6];
        uint64_t occupied;
        uint64_t white_occupied;
        uint64_t black_occupied;

        bool side; 
        
        int ep_square;
        CastlingRight castling;
        
        std::vector<BoardState> history;
        
        int getPieceType(int color, int square);
        
        void saveCurrentState();
        void updateOccupancy();
        void specialMoveHandler(int from, int to, int flag);
        void promoMoveHandler(int to_mask, int flag);
        void nonEnPassantCaptureHanlder(int to_mask);
        void revokeCastlingRights();
        void revokeRookSideCastlingRight(int color, int from);
        

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

        int getEpSquare() const { return ep_square; }

        bool canCastleKingSide(int color) const { return castling.king_side[color]; }
        bool canCastleQueenSide(int color) const { return castling.queen_side[color]; }

        void makeMove(const Move &move);
        void undoMove();
};

#endif