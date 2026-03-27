#include "bitboard.h"
#include "move.h"
#include "movegen.h"
#include "perft.h"

#include <iostream>

int main() {
    Bitboard bitboard;
    MoveGenerator movegen;

    for(int depth = 1; depth <= 4; depth++)
    {
        long nodes = perft(bitboard, movegen, depth);
        std::cout << "Depth " << depth << ": " << nodes << std::endl;
    }
    
    return 0;
}