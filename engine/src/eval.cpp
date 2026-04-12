#include <vector>
#include <limits>

#include "eval.h"

Eval::Eval(Bitboard &bitboard, MoveGenerator &movegen) 
    : bitboard(bitboard), movegen(movegen), depth(0), node_count(0) {}

int Eval::evaluate() {
    int score = 0;

    for(int piecetype = PAWNS; piecetype <= KING; piecetype++) {
        uint64_t whiteBB = bitboard.getPieces(WHITE, piecetype);
        uint64_t blackBB = bitboard.getPieces(BLACK, piecetype);
        
        // Material count
        score += MATERIAL[piecetype] * (popcount(whiteBB) - popcount(blackBB));

        // White piece square table
        uint64_t tmp = whiteBB;
        while(tmp) {
            int sq = __builtin_ctzll(tmp);
            score += PIECE_SQUARE_TABLE[piecetype][flip(sq)];
            tmp &= tmp - 1;
        }

        // Black piece square table (negated because it's from black's perspective)
        tmp = blackBB;
        while(tmp) {
            int sq = __builtin_ctzll(tmp);
            score -= PIECE_SQUARE_TABLE[piecetype][flip(sq)];
            tmp &= tmp - 1;
        }
    }

    // Double Pawn Penalty (removes 20 points per double pawns)
    static constexpr uint64_t FILE_MASK[8] = {
        0x0101010101010101ULL, 0x0202020202020202ULL,
        0x0404040404040404ULL, 0x0808080808080808ULL,
        0x1010101010101010ULL, 0x2020202020202020ULL,
        0x4040404040404040ULL, 0x8080808080808080ULL
    };

    for(int file = 0; file < 8; file++) {
        int w_pawns = popcount(bitboard.getPieces(WHITE, PAWNS) & FILE_MASK[file]);
        int b_pawns = popcount(bitboard.getPieces(BLACK, PAWNS) & FILE_MASK[file]);
        if(w_pawns > 1) score -= 20 * (w_pawns - 1);
        if(b_pawns > 1) score += 20 * (b_pawns - 1);
    }

    return score;
}


int Eval::minimax(Move *best_move) {
    // Safety checks
    if(depth <= 0) return evaluate();
    if(node_count >= MAX_NODES) return evaluate();  // Hit node limit, return early
    
    node_count++;

    std::vector<Move> moves;
    moves.reserve(80);  // Pre-allocate to avoid repeated allocations
    
    bool side = bitboard.getMovingSide();
    bool isInCheck = bitboard.isKingInCheck(side);

    movegen.generateMoves(bitboard, moves);

    if(moves.empty()) {
        if(isInCheck) {
            return side == WHITE ? -INF : INF;
        }

        return 0;  // Stalemate
    }

    depth--;  // Decrement at start

    if(side == WHITE) {
        int best = -INF;
        
        for(const Move &move : moves) {
            bitboard.makeMove(move);
            int score = minimax(nullptr);
            bitboard.undoMove();
            
            if(score > best) {
                best = score;
                if(best_move) *best_move = move;
            }
        }

        depth++;  // Restore depth for parent
        return best;
    }

    int best = INF;

    for(const Move &move : moves) {
         bitboard.makeMove(move);
         int score = minimax(nullptr);
         bitboard.undoMove();

         if(score < best) {
            best = score;
            if(best_move) *best_move = move;
         }
    }

    depth++;  // Restore depth for parent
    return best;
}

Move Eval::search(int search_depth) {
    Move best = Move(0, 0, QUIET);
    depth = search_depth;
    node_count = 0;  // Reset node counter for this search
    minimax(&best);
    return best;
}