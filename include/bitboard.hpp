#pragma once

#include <cstdint> 
#include <vector>
#include <iostream>
#include <sstream>
#include <string>

#include "move.hpp"
#include "types.hpp"

struct CastlingRight {
    bool ks[2];
    bool qs[2];
};

struct BoardState {
    uint64_t      pieces[2][6];
    bool          side;
    int           ep_square;
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
        void initializeSTARTPOSpiecesBB();

    public:
        Bitboard();
       
        void printBitboard(const int color, const int type);
        void visualizeBoard();
        void makeMove(const Move &move);
        void undoMove();
        void loadFEN(const std::string &fen);
        void removePiece(int color, int piece, uint64_t mask);
        void restorePiece(int color, int piece, uint64_t mask);
       
        void setPieces(const int color, const int type, uint64_t pieces) { 
            piecesBB[color][type] = pieces; 
        }
       
        void setMovingSide(bool value) { 
            side = value; 
        }
        
        void setEpSquare(int square) {
            ep_square = square; 
        }
        
        void setCastlingRights(CastlingRight &rights) { 
            castling = rights; 
        }        
        
        uint64_t getPieces(const int color, const int type) const { 
            return piecesBB[color][type]; 
        }
        
        uint64_t getOccupied() const { 
            return occupied; 
        }

        uint64_t getWhiteOccupied() const { 
            return white_occupied; 
        }

        uint64_t getBlackOccupied() const { 
            return black_occupied; 
        }
        
        int getEpSquare() const { 
            return ep_square; 
        }
        
        bool getMovingSide() const {
             return side; 
        }

        bool canCastleKingSide(int color) const { 
            return castling.ks[color]; 
        }

        bool canCastleQueenSide(int color) const { 
            return castling.qs[color]; 
        }
        
        bool isSquareAttacked(int square, int color) const;
        bool isKingInCheck(int color) const;
};
