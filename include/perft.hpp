#pragma once

#include <cstdint>
#include <string>

#include "bitboard.hpp"
#include "movegen.hpp"

// Inpsired by this beauty: https://www.chessprogramming.org/Perft_Results
//
// The perft methods allows us to debbug our giant codebase. Since the engine
// has so many many moving pieces (the pun was very much intended), we have to make sure
// that our program is properly generating every possible moves.

struct PerftStats {
    uint64_t nodes            = 0;
    uint64_t captures         = 0;
    uint64_t en_passants      = 0;
    uint64_t castles          = 0;
    uint64_t promotions       = 0;
    uint64_t checks           = 0;
    uint64_t discovery_checks = 0;
    uint64_t double_checks    = 0;
    uint64_t checkmates       = 0;

    PerftStats operator+(const PerftStats &other) const {
        PerftStats result;

        result.nodes            = nodes            + other.nodes;
        result.captures         = captures         + other.captures;
        result.en_passants      = en_passants      + other.en_passants;
        result.castles          = castles          + other.castles;
        result.promotions       = promotions       + other.promotions;
        result.checks           = checks           + other.checks;
        result.discovery_checks = discovery_checks + other.discovery_checks;
        result.double_checks    = double_checks    + other.double_checks;
        result.checkmates       = checkmates       + other.checkmates;

        return result;
    }

    PerftStats &operator+=(const PerftStats &other) {
        *this = *this + other;
        return *this;
    }
};

class Perft {
    private:
        MoveGenerator movegen;
        
        uint64_t   perftRecurse(Bitboard &bitboard, int depth);
        PerftStats perftStatsRecurse(Bitboard &bitboard, int depth);

    public:
        void runPerft(Bitboard &bitboard, int depth);
        void runPerftDivide(Bitboard &bitboard, int depth);
        void runPerftStats(Bitboard &bitboard, int depth);
        PerftStats perfStats(Bitboard &bitboard, int depth);
};