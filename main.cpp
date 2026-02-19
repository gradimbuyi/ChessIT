#include "headers/Board.h"
#include "headers/MoveGenerator.h"
#include "headers/utils.h"

int main() {
    ChessBoard board;
    board.printBitboard(true, PAWN);

    std::vector<Move> moves;
    MoveGenerator::generatePawnMoves(board, moves);

    printMoves(moves);
    std::cout << "reached";
    
    return 0;
}