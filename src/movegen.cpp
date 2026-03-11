#include "movegen.h"

void MoveGenerator::generateMoves(const Bitboard &bitboard, std::vector<Move> &moves) {
    pawnMoves(bitboard, moves);
    knightMoves(bitboard, moves);
    bishopMoves(bitboard, moves);
    rookMoves(bitboard, moves);
    queenMoves(bitboard, moves);
    kingMoves(bitboard, moves);
}

void MoveGenerator::pawnMoves(const Bitboard &bitboard, std::vector<Move> &moves) {
    bool side = bitboard.getMovingSide();

    uint64_t pawns = bitboard.getPieces(side, PAWNS);
    uint64_t friendly = side ? bitboard.getWhiteOccupied() : bitboard.getBlackOccupied();
    uint64_t enemy    = side ? bitboard.getBlackOccupied() : bitboard.getWhiteOccupied();
    uint64_t occupied = bitboard.getOccupied();

    int forward = side ? 8 : -8;

    while(pawns) {
        int from = __builtin_ctzll(pawns);
        int rank = from >> 3;
        int file = from & 7;
        int to = from + forward;

        if(to >= 0 && to < 64) {
            uint64_t target = 1ULL << to;

            if(!(target & occupied)) {
                moves.push_back(Move(from, to, QUIET));

                if((side && rank == 1) || (!side && rank == 6)) {
                    int to2 = from + (2 * forward);
                    uint64_t target2 = 1ULL << to2;

                    if(!(target2 & occupied)) {
                        moves.push_back(Move(from, to2, DOUBLE_PAWN_PUSH));
                    }
                }
            }
        }

        int capture_left = side ? from + 7 : from - 9;

        if(file > 0 && capture_left >= 0 && capture_left < 64) {
            uint64_t target = 1ULL << capture_left;

            if(target & enemy) {
                moves.push_back(Move(from, capture_left, CAPTURES));
            }
        }

        int capture_right = side ? from + 9 : from - 7;

        if(file < 7 && capture_right >= 0 && capture_right < 64) {
            uint64_t target = 1ULL << capture_right;

            if(target & enemy) {
                moves.push_back(Move(from, capture_right, CAPTURES));
            }
        }

        pawns &= pawns - 1;
    }
}

void MoveGenerator::knightMoves(const Bitboard &bitboard, std::vector<Move> &moves) {
    bool side = bitboard.getMovingSide(); 
    uint64_t knights = bitboard.getPieces(side, KNIGHTS);
    uint64_t friendly = side ? bitboard.getWhiteOccupied() : bitboard.getBlackOccupied(); 
    uint64_t enemy = side ? bitboard.getBlackOccupied() : bitboard.getWhiteOccupied(); 
    
    int candidates[] = {6, 10, 15, 17, -6, -10, -15, -17};
    uint64_t edges[] = {
        0xFCFCFCFCFCFCFCFCULL, 
        0xF3F3F3F3F3F3F3F3ULL, 
        0x7F7F7F7F7F7F7F7FULL, 
        0xFEFEFEFEFEFEFEFEULL
    };

    while(knights) {
        int from = __builtin_ctzll(knights);

        for(int i = 0; i < 8; i++) {
            int to = from + candidates[i];
            if(to < 0 || to > 63) continue;
            if(!(edges[i >> 1] & (1ULL << from))) continue;

            uint64_t target = 1ULL << to;
            if(target & friendly) continue;
            Flags flag = (target & enemy) ? CAPTURES : QUIET;
            moves.push_back(Move(from, to, flag));
        }

        knights &= knights - 1;
    }
}

void MoveGenerator::kingMoves(const Bitboard &bitboard, std::vector<Move> &moves) {
    bool side = bitboard.getMovingSide();
    uint64_t king = bitboard.getPieces(side, KING);
    uint64_t friendly = side ? bitboard.getWhiteOccupied() : bitboard.getBlackOccupied();
    uint64_t enemy = side ? bitboard.getBlackOccupied() : bitboard.getWhiteOccupied();

    int candidates[] = {1, 7, 8, 9, -1, -7, -8, -9};

    int from = __builtin_ctzll(king);
    int from_rank = from >> 3;
    int from_file = from & 7;
    
    for(int candidate : candidates) {
        int to = from + candidate;
        if(to < 0 || to > 63) continue;

        int to_rank = to >> 3;
        int to_file = to & 7;

        if(abs(to_rank - from_rank) > 1 || abs(to_file - from_file) > 1) continue;
        
        uint64_t target = 1ULL << to;
        if(target & friendly) continue;

        Flags flag = (target & enemy) ? CAPTURES : QUIET;
        moves.push_back(Move(from, to, flag));
    }  
}

void addSlidingMoves(const Bitboard &bitboard, uint64_t pieces, const int (&directions)[8], 
                     bool side, std::vector<Move> &moves) {
    uint64_t friendly = side ? bitboard.getWhiteOccupied() : bitboard.getBlackOccupied();
    uint64_t enemy = side ? bitboard.getBlackOccupied() : bitboard.getWhiteOccupied();
    
    while(pieces) {
        int from = __builtin_ctzll(pieces);
        int from_rank = from >> 3;
        int from_file = from & 7;

        for(int direction : directions) {
            if(direction == 0) continue;
            int to = from;
           
            while(true) {
                int prev = to;
                
                to += direction;
                
                if(to < 0 || to > 63) break;

                int prev_rank = prev >> 3;
                int prev_file = prev & 7;
                int to_rank = to >> 3;
                int to_file = to & 7;

                if(abs(to_rank - prev_rank) > 1 || abs(to_file - prev_file) > 1) break;

                uint64_t target = 1ULL << to;
                if(target & friendly) break;

                Flags flag = (target & enemy) ? CAPTURES : QUIET;
                moves.push_back(Move(from, to, flag));

                if(target & enemy) break;
            }
        }
        pieces &= pieces - 1;
    }   
}

void MoveGenerator::bishopMoves(const Bitboard &bitboard, std::vector<Move> &moves) {
    bool side = bitboard.getMovingSide();
    uint64_t bishops = bitboard.getPieces(side, BISHOPS);
    int directions[8] = {7, 9, -7, -9, 0, 0, 0, 0};

    addSlidingMoves(bitboard, bishops, directions, side, moves);
}

void MoveGenerator::rookMoves(const Bitboard &bitboard, std::vector<Move> &moves) {
    bool side = bitboard.getMovingSide();
    uint64_t rooks = bitboard.getPieces(side, ROOKS);
    int directions[8] = {1, -1, 8, -8, 0, 0, 0, 0};

    addSlidingMoves(bitboard, rooks, directions, side, moves);
}

void MoveGenerator::queenMoves(const Bitboard &bitboard, std::vector<Move> &moves) {
    bool side = bitboard.getMovingSide();
    uint64_t queens = bitboard.getPieces(side, QUEENS);
    int directions[8] = {1, -1, 8, -8, 7, 9, -7, -9};
    
    addSlidingMoves(bitboard, queens, directions, side, moves);
}