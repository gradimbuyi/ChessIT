#include <iostream>
#include <sstream>
#include <string>

#include "console.h"
#include "ucicomm.h"

Console::Console(Bitboard &bitboard) : bitboard(bitboard) {
    bitboard.loadFEN(STARTPOS);
}

void Console::printHelpMenu() {
    std::cout << "\nCommands: \n";
    std::cout << "  perft <depth>           - run perft to depth N\n";
    std::cout << "  perft divide <depth>    - run perft divide at depth N\n";
    std::cout << "  perft stats <depth>     - get perft stats at depth N\n";
    std::cout << "  load <fen>              - load a position from FEN\n";
    std::cout << "  display                 - print current board\n";
    std::cout << "  reset                   - reset to starting position\n";
    std::cout << "  help                    - show this menu\n";
    std::cout << "  uci                     - switches into UCI mode\n";
    std::cout << "  exit                    - quit\n\n";
}

void Console::perftHandler(std::istringstream &iss) {
    std::string next;
    iss >> next;

    if(next == "divide" || next == "stats") {
        int depth;

        if(!(iss >> depth) || depth < 1) {
            std::cout << "Usage: perft " << next << " <depth>\n";
            return;
        }

        if(next == "divide") perft.runPerftDivide(bitboard, depth);
        else perft.runPerftStats(bitboard, depth);
        
        return;
    }

    int depth;
    
    try {
        depth = std::stoi(next);
    } catch(...) {
        std::cout << "Usage: perft <depth>\n";
        return;
    }

    if(depth < 1) {
        std::cout << "Depth must be at least 1.\n";
        return;
    }

    perft.runPerft(bitboard, depth);
}

void Console::fenLoadingHandler(std::istringstream &iss) {
    std::string fen;
    std::getline(iss, fen);

    if(!fen.empty() && fen[0] == ' ') fen = fen.substr(1);

    if(fen.empty()) {
        std::cout << "Usage: load <fen>\n";
        return;
    }

    bitboard.loadFEN(fen);
    std::cout << "Position loaded...\n";
}

void Console::runEngineCLI() {
    std::cout << "Chess Engine - type 'help' for commands\n";
    
    std::string line;

    while(true) {
        std::cout << "> ";
        
        if(!std::getline(std::cin, line)) break;
        if(line.empty()) continue;
        
        std::istringstream iss(line);
        std::string command;
        iss >> command;

        if(command == "exit" || command == "quit") {
            std::cout << "Goodbye.\n";
            std::cout << "Author: Gradi Tshielekeja Mbuyi, Contact: gradimbuyi@outlook.com\n";
            break;
        }

        else if(command == "help") {
            printHelpMenu();
        }

        else if(command == "display") {
            bitboard.visualizeBoard();
        }

        else if(command == "load") {
            fenLoadingHandler(iss);
        }

        else if(command == "perft") {
            perftHandler(iss);
        }

        else if(command == "uci") {
            UCIComm uci(bitboard);
            uci.run();
        }

        else {
            std::cout << "Unknown command '" << command << "'. Type 'help' for commands.\n";
        }
    }
}