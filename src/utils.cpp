#include "../headers/utils.h"

void printMoves(std::vector<Move> &moves) {
    for (const auto &move : moves) {
        if(move.piece == PAWN) {
            std::cout << "Pawn at " << move.moving_from << " can move to " << move.moving_to << std::endl; 
        }
    }
}