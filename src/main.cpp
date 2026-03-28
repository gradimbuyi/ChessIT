#include "bitboard.h"
#include "move.h"
#include "movegen.h"
#include "perft.h"

#include <iostream>

int main() {
    Perft perft;
    Bitboard bitboard;
    
     // --- Test 1: Starting position ---
    bitboard.loadFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    perft.runPerft(bitboard, 5, "perft_result.txt");

    //bitboard.visualizeBoard();

    //std::cout << "White occupied: " << bitboard.getWhiteOccupied() << "\n";
    //std::cout << "Black occupied: " << bitboard.getBlackOccupied() << "\n";
    //std::cout << "Occupied:       " << bitboard.getOccupied()      << "\n";

     // --- Test 2: Divide at depth 3 for debugging ---
    bitboard.loadFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    perft.runPerftDivide(bitboard, 3, "perft_divide_result.txt");

}