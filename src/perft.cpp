#include <iostream>
#include <iomanip>
#include <chrono>

#include "../include/perft.hpp"

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

void Perft::runPerft(Bitboard &bitboard, int depth) {
    std::cout << "Running perft to depth " << depth << "...\n\n";

    for(int i = 0; i <= depth; i++) {
        auto start = std::chrono::high_resolution_clock::now();

        uint64_t nodes = perftRecurse(bitboard, i);

        auto end = std::chrono::high_resolution_clock::now();
        auto ms  = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();

        std::cout << "Depth " << i << " Nodes: " << std::setw(14) << nodes << " Time: " << ms << "ms\n";
    }

    std::cout << "\n";
}

void Perft::runPerftDivide(Bitboard &bitboard, int depth) {
    std::cout << "Perft divide at depth " << depth << "...\n\n";

    std::vector<Move> moves;

    movegen.generateMoves(bitboard, moves);

    uint64_t nodes_searched = 0;

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

        std::cout << from_file << from_rank << to_file << to_rank;

        switch(flag) {
            case KNIGHT_PROMOTION : case KNIGHT_PROMO_CAPTURE : std::cout << 'n'; break;
            case BISHOP_PROMOTION : case BISHOP_PROMO_CAPTURE : std::cout << 'b'; break;
            case ROOK_PROMOTION   : case ROOK_PROMO_CAPTURE   : std::cout << 'r'; break;
            case QUEEN_PROMOTION  : case QUEEN_PROMO_CAPTURE  : std::cout << 'q'; break;
            default: break;
        }

        std::cout << ": " << nodes << "\n";
        nodes_searched += nodes;
    }

    std::cout << "\nNodes searched: " << nodes_searched << "\n";
}

PerftStats Perft::perftStatsRecurse(Bitboard &bitboard, int depth) {
    PerftStats stats;

    std::vector<Move> moves;
    
    movegen.generateMoves(bitboard, moves);

    if(moves.empty()) return stats;
    if(depth == 0) { stats.nodes = 1; return stats; }

    for(const Move &move : moves) {
        int flag = move.getFlag();
        int side = bitboard.getMovingSide();

        bitboard.makeMove(move);

        if(depth == 1) {
            stats.nodes++;

            if(move.isCapture())                            stats.captures++;
            if(flag == EN_CAPTURES)                         stats.en_passants++;
            if(flag == KING_CASTLE || flag == QUEEN_CASTLE) stats.castles++;
            if(flag >= KNIGHT_PROMOTION)                    stats.promotions++;

            int  enemy    = !side;
            bool in_check = bitboard.isKingInCheck(enemy);

            if(in_check) {
                std::vector<Move> enemy_moves;

                movegen.generateMoves(bitboard, enemy_moves);

                if(enemy_moves.empty()) stats.checkmates++;
                
                stats.checks++;
            } 
        }
        
        else {
            PerftStats child = perftStatsRecurse(bitboard, depth - 1);
            stats += child;
        } 

        bitboard.undoMove();
    }

    return stats;
}

void Perft::runPerftStats(Bitboard &bitboard, int depth) {
    std::cout << "\nPerft stats at depth " << depth << ":\n\n";

    auto       start = std::chrono::high_resolution_clock::now();
    PerftStats stats = perftStatsRecurse(bitboard, depth);
    auto       end   = std::chrono::high_resolution_clock::now();
    auto       ms    = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();

    std::cout << "Nodes:       " << std::setw(14) << stats.nodes << "\n";
    std::cout << "Captures:    " << std::setw(14) << stats.captures << "\n";
    std::cout << "En passants: " << std::setw(14) << stats.en_passants << "\n";
    std::cout << "Castles:     " << std::setw(14) << stats.castles << "\n";
    std::cout << "Promotions:  " << std::setw(14) << stats.promotions << "\n";
    std::cout << "Checks:      " << std::setw(14) << stats.checks << "\n";
    std::cout << "Checkmates:  " << std::setw(14) << stats.checkmates << "\n";
    std::cout << "Time:        " << std::setw(14) << ms << "ms\n\n";
}

PerftStats Perft::perfStats(Bitboard &bitboard, int depth) {
    return perftStatsRecurse(bitboard, depth);
}