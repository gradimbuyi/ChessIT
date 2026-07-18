#pragma once

#include <cstdlib>
#include <ctime>

#include "bitboard.hpp"
#include "movegen.hpp"
#include "eval.hpp"

class UCIComm {
    private:
        Bitboard &bitboard;
        MoveGenerator movegen;
        
        const std::string ENGINE_NAME  = "ChessIT";
        const std::string AUTHROR_NAME = "Gradi Mbuyi";
        const std::string STARTPOS     = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
        
        void positionHandler(std::istringstream &iss);
        void goHandler(std::istringstream &iss);
        Move parseMoveString(const std::string &move_string);
        Move pickRandomMove();
        Move pickBestMove(int depth = 3); 

    public:
        UCIComm(Bitboard &bitboard);
        void run();
};