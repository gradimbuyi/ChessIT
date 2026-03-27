#include "perft.h"
#include "movegen.h"

#include <vector>

long perft(Bitboard &bitboard, MoveGenerator &movegen, int depth) {
    if(depth == 0) return 1;

    std::vector<Move> moves;

    movegen.generateMoves(bitboard, moves);

    long nodes = 0;

    for(const Move &move : moves) {
        bitboard.makeMove(move);
        nodes += perft(bitboard, movegen, depth - 1);
        bitboard.undoMove();
    }

    return nodes;
}