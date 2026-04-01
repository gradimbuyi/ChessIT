#include "bitboard.h"
#include "console.h"
#include "ucicomm.h"

int main(int argc, char *argv[]) {
    Bitboard bitboard;

    if(argc > 1 && std::string(argv[1]) == "uci") {
        UCIComm uci(bitboard);
        uci.run();

        return 0;
    }

    Console console(bitboard);
    console.runEngineCLI();

    return 0;
}