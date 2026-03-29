#ifndef BITBOARD_H
#define BITBOARD_H

#include <cstdint> 
#include <string>
#include <vector>

#include "move.h"

enum bbtype { KING, QUEENS, ROOKS, BISHOPS, KNIGHTS, PAWNS};
enum colors { WHITE, BLACK };

struct CastlingRight {
    bool king_side[2];
    bool queen_side[2];
};

struct BoardState {
    uint64_t pieces[2][6];
    bool side;
    int ep_square;
    CastlingRight castling;
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
        void promoMoveHandler(uint64_t to_mask, int flag);
        void nonEnPassantCaptureHanlder(uint64_t to_mask);
        void revokeCastlingRights();
        void revokeRookSideCastlingRight(int color, int from);
        void resetState();
        void initizializeSTARTPOSpiecesBB();

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
        void setEpSquare(int square) { ep_square = square; }
        void setCastlingRights(CastlingRight &rights) { castling = rights; }
       
        int getEpSquare() const { return ep_square; }
        int getPieceTypePublic(int color, int square);
        
        bool getMovingSide() const { return side; }

        bool canCastleKingSide(int color)  const { return castling.king_side[color]; }
        bool canCastleQueenSide(int color) const { return castling.queen_side[color]; }
        bool isSquareAttacked(int square, int color) const;
        bool isKingInCheck(int color) const;

        void makeMove(const Move &move);
        void undoMove();
        void loadFEN(const std::string &fen);
        void updateOccupancyPublic();
        void tempRemovePiece(int color, int piece, uint64_t mask);
        void tempRestorePiece(int color, int piece, uint64_t mask);
};

#endif