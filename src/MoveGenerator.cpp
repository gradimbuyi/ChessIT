#include "../headers/MoveGenerator.h"

void pawnAttack(std::vector<Move> &moves, int from, int to, uint64_t enemy, uint64_t promotion_rank) {
    uint64_t to_bit = 1ULL << to;

    if(enemy & to_bit) {
        if(promotion_rank & to_bit) {
            for(int promotion : {QUEEN, ROOK, BISHOP, KNIGHT}) {
                Move move = {from, to, PAWN, NONE, promotion, false, false};
                moves.push_back(move);
            }
        } else {
            Move move = {from, to, PAWN, NONE, NONE, false, false};
            moves.push_back(move);
        }
    }
}

void pawnPush(std::vector<Move> &moves, int from, int to, uint64_t to_bit, uint64_t promotion_rank) {
    if(promotion_rank & to_bit) {
        for(int promotion : {QUEEN, ROOK, BISHOP, KNIGHT}) {
            Move move = {from, to, PAWN, NONE, promotion, false, false};
            moves.push_back(move);
        }
    } else {
        Move move = {from, to, PAWN, NONE, NONE, false, false};
        moves.push_back(move);
    }
}

void MoveGenerator::generatePawnMoves(const ChessBoard &board, std::vector<Move> &moves) {
    uint64_t white    = board.isWhiteTurn();
    uint64_t occupied = board.getOccupied();
    uint64_t pawns          = white ? board.getWhitePawns()    : board.getBlackPawns();
    uint64_t promotion_rank = white ? WHITE_PROMOTION_RANK     : BLACK_PROMOTION_RANK;
    uint64_t start_rank     = white ? WHITE_PAWNS_START_RANK   : BLACK_PAWNS_START_RANK;
    uint64_t enemy          = white ? board.getBlackOccupied() : board.getWhiteOccupied();
    uint64_t current        = pawns;

    int push = white ? 8 : -8;   

    while(current) {
        int from = __builtin_ctzll(current);
        int to   = from + push;
        uint64_t from_bit = 1ULL << from;
        uint64_t to_bit   = 1ULL << to;

        if(!(to_bit & occupied)) {
            pawnPush(moves, from, to, to_bit, promotion_rank);

            if(start_rank & from_bit) {
                int to2 = from + push * 2;
                int to2_bit = 1ULL << to2;
                
                if(!(to2_bit & occupied)) {
                    pawnPush(moves, from, to2, to2_bit, promotion_rank);
                }
            }
        }

        if(from_bit & NOT_A_FILE) {
            int to = from + push - 1;
            pawnAttack(moves, from, to, enemy, promotion_rank);
        }

        if(from_bit & NOT_H_FILE) {
            int to = from + push + 1;
            pawnAttack(moves, from, to, enemy, promotion_rank);
        }

        current &= current - 1;
    }
}