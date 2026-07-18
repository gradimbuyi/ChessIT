#pragma once

#include <string>

#include "bitboard.hpp"
#include "perft.hpp"

class Console {
    private:
        Bitboard &bitboard;
        Perft perft;

        const std::string STARTPOS = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

        void perftHandler(std::istringstream &iss);
        void fenLoadingHandler(std::istringstream &iss);
        void printHelpMenu();

    public:
        Console(Bitboard &bitboard);
        void runEngineCLI();
};