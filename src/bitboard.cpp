#include <iostream>
#include "bitboard.h"

Bitboard::Bitboard() { 
    piecesBB[WHITE][PAWNS]   = 0x000000000000FF00ULL;
    piecesBB[WHITE][KNIGHTS] = 0x0000000000000042ULL;
    piecesBB[WHITE][BISHOPS] = 0x0000000000000024ULL;
    piecesBB[WHITE][ROOKS]   = 0x0000000000000081ULL;
    piecesBB[WHITE][QUEENS]  = 0x0000000000000008ULL;
    piecesBB[WHITE][KING]    = 0x0000000000000010ULL;
    piecesBB[BLACK][PAWNS]   = 0x00FF000000000000ULL;
    piecesBB[BLACK][KNIGHTS] = 0x4200000000000000ULL;
    piecesBB[BLACK][BISHOPS] = 0x2400000000000000ULL;    
    piecesBB[BLACK][ROOKS]   = 0x8100000000000000ULL;
    piecesBB[BLACK][QUEENS]  = 0x0800000000000000ULL;
    piecesBB[BLACK][KING]    = 0x1000000000000000ULL;

    white_occupied = 
        piecesBB[WHITE][PAWNS] | piecesBB[WHITE][KNIGHTS] | piecesBB[WHITE][BISHOPS] | 
        piecesBB[WHITE][ROOKS] | piecesBB[WHITE][QUEENS]  | piecesBB[WHITE][KING];

    black_occupied = 
        piecesBB[BLACK][PAWNS] | piecesBB[BLACK][KNIGHTS] | piecesBB[BLACK][BISHOPS] | 
        piecesBB[BLACK][ROOKS] | piecesBB[BLACK][QUEENS]  | piecesBB[BLACK][KING];

    occupied = white_occupied | black_occupied;

    side = WHITE;
    
    castled[WHITE][KING] = false;
    castled[WHITE][QUEENS] = false;
    castled[BLACK][KING] = false;
    castled[BLACK][QUEENS]= false;
}

void Bitboard::printBitboard(int color, int type) {
    uint64_t bb = getPieces(color, type);

    if((color > 1 || color < 0) && (type > 5 || type < 0)) {
        std::cout << "Invalid inputs. Could not print out bitboard" << std::endl << std::endl;
        return ;
    }

    std::cout << "Priting bitboard..." << std::endl << std::endl;

    for(int rank = 7; rank >= 0; rank--) {
        std::cout << rank + 1 << " ";
        for(int file = 0; file < 8; file++) {
            int sq = rank * 8 + file;
            if ((bb >> sq) & 1) {
                std::cout << "1 ";
            } else {
                std::cout << "0 ";
            }
        }
        std::cout << "\n";
    }

    std::cout << "\n   a b c d e f g h\n\n";
}

void Bitboard::visualizeBoard() {
    std::cout << "Printing Chess Board..." << std::endl << std::endl;

    for(int rank = 7; rank >= 0; rank--) {
        std::cout << rank + 1 << "  ";
        for(int file = 0; file < 8; file++) {
            int sq = rank * 8 + file;
            
            uint64_t bit = 1ULL << sq;
            
            char p = '.';

            uint64_t (*bb)[6] = piecesBB;
            
                 if(bb[WHITE][PAWNS]   & bit) p = 'P';
            else if(bb[WHITE][KNIGHTS] & bit) p = 'N';
            else if(bb[WHITE][BISHOPS] & bit) p = 'B';
            else if(bb[WHITE][ROOKS]   & bit) p = 'R';
            else if(bb[WHITE][QUEENS]  & bit) p = 'Q';
            else if(bb[WHITE][KING]    & bit) p = 'K';
            else if(bb[BLACK][PAWNS]   & bit) p = 'p';
            else if(bb[BLACK][KNIGHTS] & bit) p = 'n';
            else if(bb[BLACK][BISHOPS] & bit) p = 'b';
            else if(bb[BLACK][ROOKS]   & bit) p = 'r';
            else if(bb[BLACK][QUEENS]  & bit) p = 'q';
            else if(bb[BLACK][KING]    & bit) p = 'k';

            std::cout << p << " ";
        }
        std::cout << std::endl;
    }
    std::cout <<  std::endl << "   a b c d e f g h" << std::endl << std::endl;
}
