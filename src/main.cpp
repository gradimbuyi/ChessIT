#include "bitboard.h"
#include "move.h"
#include "movegen.h"

#include <iostream>

int main() {
    Bitboard bitboard;
    std::vector<Move> moves;

    MoveGenerator::generateMoves(bitboard, moves);

    std::cout << "Moves generated: " << moves.size() << std::endl;
    
    return 0;
}