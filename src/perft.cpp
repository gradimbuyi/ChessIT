#include <fstream>
#include <iostream>

#include <iomanip>
#include <chrono>

#include "perft.h"

uint64_t Perft::perftRecurse(Bitboard &bitboard, int depth) {
    if(depth == 0) return 1;

    std::vector<Move> moves;
   
    movegen.generateMoves(bitboard, moves);

    if(depth == 1) return moves.size();

    uint64_t nodes = 0;

    for(const Move &move : moves) {
        bitboard.makeMove(move);
        nodes += perftRecurse(bitboard, depth - 1);
        bitboard.undoMove();
    }

    return nodes;
}

void Perft::runPerft(Bitboard &bitboard, int depth, const std::string &filename) {
    std::ostream *out = &std::cout;
    std::ofstream file;

    if(!filename.empty()) {
        file.open(filename);
        if(!file.is_open()) {
            std::cerr << "Failed to open file: " << filename << "\n";
            return;
        }
        out = &file;
    }

    *out << "Running perft to depth " << depth << "...\n\n";

    uint64_t total = 0;

    for(int i = 0; i < depth; i++) {
        auto start = std::chrono::high_resolution_clock::now();

        uint64_t nodes = perftRecurse(bitboard, i);

        auto end = std::chrono::high_resolution_clock::now();
        auto ms  = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();

        *out << "Depth " << i << " Nodes: " << std::setw(14) << nodes << " Time: " << ms << "ms\n";
    }

    *out << "\n";
}

void Perft::runPerftDivide(Bitboard &bitboard, int depth, const std::string &filename) {
    std::ostream *out = &std::cout;
    std::ofstream file;

    if(!filename.empty()) {
        file.open(filename);
        if(!file.is_open()) {
            std::cerr << "Failed to open file: " << filename << "\n";
            return;
        }
        out = &file;
    }

    *out << "Perft divide at depth " << depth << "...\n\n";

    std::vector<Move> moves;

    movegen.generateMoves(bitboard, moves);

    uint64_t total = 0;

    for(const Move &move : moves) {
        bitboard.makeMove(move);
        uint64_t nodes = perftRecurse(bitboard, depth - 1);
        bitboard.undoMove();

        int  from      = move.getFrom();
        int  to        = move.getTo();
        int  flag      = move.getFlag();
        char from_file = 'a' + (from & 7);
        char from_rank = '1' + (from >> 3);
        char to_file   = 'a' + (to & 7);
        char to_rank   = '1' + (to >> 3);

        *out << from_file << from_rank << to_file << to_rank;

        switch(flag) {
            case KNIGHT_PROMOTION : case KNIGHT_PROMO_CAPTURE : std::cout << 'n'; break;
            case BISHOP_PROMOTION : case BISHOP_PROMO_CAPTURE : std::cout << 'b'; break;
            case ROOK_PROMOTION   : case ROOK_PROMO_CAPTURE   : std::cout << 'r'; break;
            case QUEEN_PROMOTION  : case QUEEN_PROMO_CAPTURE  : std::cout << 'k'; break;
            default: break;
        }

        *out << ": " << nodes << "\n";
        total += nodes;
    }

    *out << "\nTotal: " << total << "\n";
}