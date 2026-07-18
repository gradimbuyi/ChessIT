#include <iostream>
#include "../include/movegen.hpp"

void MoveGenerator::generateMoves(Bitboard &bitboard, std::vector<Move> &moves) {
    std::vector<Move> candidates;
    
    moves.clear();
    moves.reserve(80);

    candidates.reserve(80);

    pawnMoves(bitboard, candidates);
    knightMoves(bitboard, candidates);
    bishopMoves(bitboard, candidates);
    rookMoves(bitboard, candidates);
    queenMoves(bitboard, candidates);
    kingMoves(bitboard, candidates);

    int side = bitboard.getMovingSide();

    for(const Move &move : candidates) {
        bitboard.makeMove(move);
        
        if(!bitboard.isKingInCheck(side)) {
            moves.push_back(move);
        }
        bitboard.undoMove();
    }
}

void addPawnPromotions(std::vector<Move> &moves, int from, int to) {
    moves.push_back(Move(from, to, QUEEN_PROMOTION));
    moves.push_back(Move(from, to, ROOK_PROMOTION));
    moves.push_back(Move(from, to, BISHOP_PROMOTION));
    moves.push_back(Move(from, to, KNIGHT_PROMOTION));
}

void forwardPawnMoves(std::vector<Move> &moves, int from, int to, bool promotes, uint64_t occupied, int forward, int start_rank, int rank) {
    if(promotes) { addPawnPromotions(moves, from, to); return ; } 

    moves.push_back(Move(from, to, QUIET));
        
    if(rank == start_rank) {
        int to2 = from + 2 * forward;
       
        if(!((1ULL << to2) & occupied)) {
            moves.push_back(Move(from, to2, DOUBLE_PAWN_PUSH));
        }
    }
}

void pawnCaptures(std::vector<Move> &moves, int from, int to, bool side, bool promotes, uint64_t enemy, int file, int ep_square, int rank) {
    int  directions[2] = { side == WHITE ? 7 : -9,  side == WHITE ? 9 : -7 };
    int  files[2]      = { file - 1, file + 1 };
    int  ep_rank       = side == WHITE ? 4 : 3;
    bool ep_eligible   = (rank == ep_rank);

    for(int i = 0; i < 2; i++) {
        int capture_file = files[i];
        int capture_sq   = from + directions[i];

        if(capture_file < 0 || capture_file > 7)  continue;
        if(capture_sq   < 0 || capture_sq   > 63) continue;

        uint64_t capture_mask = 1ULL << capture_sq;

        if(capture_mask & enemy) {
            if(promotes) {
                addPawnPromotions(moves, from, capture_sq);
                continue;
            }

            moves.push_back(Move(from, capture_sq, CAPTURES));
        }

        if(ep_eligible && ep_square != -1 && capture_sq == ep_square) {
            moves.push_back(Move(from, capture_sq, EN_CAPTURES));
        }
    }
}

void MoveGenerator::pawnMoves(const Bitboard &bitboard, std::vector<Move> &moves) {
    bool side = bitboard.getMovingSide();

    uint64_t pawns    = bitboard.getPieces(side, PAWNS);
    uint64_t enemy    = side == WHITE ? bitboard.getBlackOccupied() : bitboard.getWhiteOccupied();
    uint64_t occupied = bitboard.getOccupied();

    int ep_square  = bitboard.getEpSquare();
    int forward    = side == WHITE ? 8 : -8;
    int promo_rank = side == WHITE ? 6 : 1;
    int start_rank = side == WHITE ? 1 : 6;

    while(pawns) {
        int from = __builtin_ctzll(pawns);
        int rank = from >> 3;
        int file = from & 7;
        int to   = from + forward;
        bool promotes = (rank == promo_rank);

        if(to >= 0 && to < 64 && !((1ULL << to) & occupied)) {
            forwardPawnMoves(moves, from, to, promotes, occupied, forward, start_rank, rank);
        }

        pawnCaptures(moves, from, to, side, promotes, enemy, file, ep_square, rank);

        pawns &= pawns - 1;
    }
}

void MoveGenerator::knightMoves(const Bitboard &bitboard, std::vector<Move> &moves) {
    bool     side     = bitboard.getMovingSide(); 
    uint64_t knights  = bitboard.getPieces(side, KNIGHTS);
    uint64_t friendly = side == WHITE ? bitboard.getWhiteOccupied() : bitboard.getBlackOccupied(); 
    uint64_t enemy    = side == WHITE ? bitboard.getBlackOccupied() : bitboard.getWhiteOccupied(); 
    
    int candidates[]  = { 6, 10, 15, 17, -6, -10, -15, -17 };

    while(knights) {
        int from      = __builtin_ctzll(knights);
        int from_file = from & 7;
        int from_rank = from >> 3;

        for(int i = 0; i < 8; i++) {
            int to = from + candidates[i];
            
            if(to < 0 || to > 63) continue;
            
            int file_diff = abs((to & 7)  - from_file);
            int rank_diff = abs((to >> 3) - from_rank);

            if(!((file_diff == 1 && rank_diff == 2) || (file_diff == 2 && rank_diff == 1))) continue;

            uint64_t target = 1ULL << to;
            
            if(target & friendly) continue;
            
            Flags flag = (target & enemy) ? CAPTURES : QUIET;

            moves.push_back(Move(from, to, flag));
        }

        knights &= knights - 1;
    }
}

void castlingMoves(const Bitboard &bitboard, std::vector<Move> &moves) {
    bool     side                  = bitboard.getMovingSide();
    int      enemy                 = !side;
    uint64_t occupied              = bitboard.getOccupied();
    uint64_t rooks                 = bitboard.getPieces(side, ROOKS);
    int      king_sq               = side == WHITE ? 4 : 60;
    int      king_side_rook_sq     = side == WHITE ? 7 : 63;
    int      queen_side_rook_sq    = side == WHITE ? 0 : 56;
    
    uint64_t king_side_empty_mask  = side == WHITE ? (1ULL << 5)  | (1ULL << 6)  : (1ULL << 61) | (1ULL << 62);
    uint64_t queen_side_empty_mask = side == WHITE ? (1ULL << 1)  | (1ULL << 2)  | (1ULL << 3) :
                                                     (1ULL << 57) | (1ULL << 58) | (1ULL << 59); 

    if(bitboard.isSquareAttacked(king_sq, enemy)) return;

    if(bitboard.canCastleKingSide(side)) {
        bool r_present  = (rooks & (1ULL << king_side_rook_sq)) != 0;
        bool path_clear = (occupied & king_side_empty_mask) == 0;
        int  transit    = side == WHITE ? 5 : 61;
        int  landing    = side == WHITE ? 6 : 62;
        bool is_safe    = !bitboard.isSquareAttacked(transit, enemy) && 
                          !bitboard.isSquareAttacked(landing, enemy);

        if(r_present && path_clear && is_safe) { 
            moves.push_back(Move(king_sq, king_sq + 2, KING_CASTLE));
        }
    }

    if(bitboard.canCastleQueenSide(side)) {
        bool r_present  = (rooks & (1ULL << queen_side_rook_sq)) != 0;
        bool path_clear = (occupied & queen_side_empty_mask) == 0;
        int  transit    = side == WHITE ? 3 : 59;
        int  landing    = side == WHITE ? 2 : 58;
        bool is_safe    = !bitboard.isSquareAttacked(transit, enemy) && 
                          !bitboard.isSquareAttacked(landing, enemy);

        if(r_present && path_clear && is_safe) {
            moves.push_back(Move(king_sq, king_sq - 2, QUEEN_CASTLE));
        }
    }
}

void MoveGenerator::kingMoves(const Bitboard &bitboard, std::vector<Move> &moves) {
    bool side = bitboard.getMovingSide();
    
    uint64_t king     =                 bitboard.getPieces(side, KING);
    uint64_t friendly = side == WHITE ? bitboard.getWhiteOccupied() : bitboard.getBlackOccupied();
    uint64_t enemy    = side == WHITE ? bitboard.getBlackOccupied() : bitboard.getWhiteOccupied();
   
    int candidates[]  = { 1, 7, 8, 9, -1, -7, -8, -9 };

    int from      = __builtin_ctzll(king);
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

    castlingMoves(bitboard, moves);
}

void addSlidingMoves(const Bitboard &bitboard, uint64_t pieces, const int (&directions)[8], bool side, std::vector<Move> &moves) {
    uint64_t friendly = side == WHITE ? bitboard.getWhiteOccupied() : bitboard.getBlackOccupied();
    uint64_t enemy    = side == WHITE ? bitboard.getBlackOccupied() : bitboard.getWhiteOccupied();
    
    while(pieces) {
        int from      = __builtin_ctzll(pieces);
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
                int to_rank   = to >> 3;
                int to_file   = to & 7;

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