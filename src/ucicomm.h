#ifndef UCICOMM_H
#define UCICOMM_H

#include <string>

#include "bitboard.h"
#include "movegen.h"

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

    public:
        UCIComm(Bitboard &bitboard);
        void run();
};

#endif