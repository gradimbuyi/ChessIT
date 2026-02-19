#include <iostream>
#include "../headers/Board.h"

ChessBoard::ChessBoard() {
    /*
     * Constructor to initialize the starting position of the chess pieces. 
     * Note:
     *  - Each piece types have a unique bitboard representating their location
     *  - Values / Variables representing the state of the game will be true at the beginning 
     *  - occupied: special bitboard that has the location of each pieces
     */
    white_pawns    = 0x000000000000FF00ULL;
    white_knights  = 0x0000000000000042ULL;
    white_bishops  = 0x0000000000000024ULL;
    white_rooks    = 0x0000000000000081ULL;
    white_queens   = 0x0000000000000008ULL;
    white_king     = 0x0000000000000010ULL;
    black_pawns    = 0x00FF000000000000ULL;
    black_knights  = 0x4200000000000000ULL;
    black_bishops  = 0x2400000000000000ULL;    
    black_rooks    = 0x8100000000000000ULL;
    black_queens   = 0x0800000000000000ULL;
    black_king     = 0x1000000000000000ULL;
    white_occupied = white_pawns | white_knights | white_bishops | white_rooks | white_queens | white_king;
    black_occupied = black_pawns | black_knights | black_bishops | black_rooks | black_queens | black_king;
    occupied       = white_occupied | black_occupied;
    en_passant_square       = 0ULL;
    white_to_move           = true;
    white_king_side_castle  = true;
    white_queen_side_castle = true;
    black_king_side_castle  = true;
    black_queen_side_castle = true;
}

void ChessBoard::printBitboard(bool is_white, int piece) {
    std::cout << "Priting bitboard..." << std::endl << std::endl;

    uint64_t bitboard;

         if(piece == PAWN)   bitboard = is_white ? white_pawns   : black_pawns;
    else if(piece == KNIGHT) bitboard = is_white ? white_knights : black_knights;
    else if(piece == BISHOP) bitboard = is_white ? white_bishops : black_bishops;
    else return; 

    for (int rank = 7; rank >= 0; rank--) {
        std::cout << rank + 1 << "  ";
        for (int file = 0; file < 8; file++) {
            int square = rank * 8 + file;
            if ((bitboard >> square) & 1) {
                std::cout << "1 ";
            } else {
                std::cout << "0 ";
            }
        }
        std::cout << "\n";
    }
    std::cout << "\n   a b c d e f g h\n\n";
}

void ChessBoard::visualizeBoard() {
    std::cout << "Printing Chess Board..." << std::endl << std::endl;

    for (int rank = 7; rank >= 0; rank--) {
        std::cout << rank + 1 << "  ";
        for (int file = 0; file < 8; file++) {
            int square   = rank * 8 + file;
            uint64_t bit = 1ULL << square;
            char piece   = '.';
            
            if(white_pawns & bit)           piece = 'P';
            else if(white_knights & bit)    piece = 'N';
            else if(white_bishops & bit)    piece = 'B';
            else if(white_rooks & bit)      piece = 'R';
            else if(white_queens & bit)     piece = 'Q';
            else if(white_king & bit)       piece = 'K';
            else if(black_pawns & bit)      piece = 'p';
            else if(black_knights & bit)    piece = 'n';
            else if(black_bishops & bit)    piece = 'b';
            else if(black_rooks & bit)      piece = 'r';
            else if(black_queens & bit)     piece = 'q';
            else if(black_king & bit)       piece = 'k';

            std::cout << piece << " ";
        }

        std::cout << std::endl;
    }

    std::cout <<  std::endl << "   a b c d e f g h" << std::endl << std::endl;
}