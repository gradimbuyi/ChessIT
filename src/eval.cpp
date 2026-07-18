#include "../include/eval.hpp"

Eval::Eval(Bitboard &bitboard, MoveGenerator &movegen) 
    : bitboard(bitboard), movegen(movegen), node_count(0) {}

int Eval::evaluate() {
    int score = 0;

    for(int piecetype = PAWNS; piecetype <= KING; piecetype++) {
        uint64_t whiteBB = bitboard.getPieces(WHITE, piecetype);
        uint64_t blackBB = bitboard.getPieces(BLACK, piecetype);
        
        score += MATERIAL[piecetype] * (popcount(whiteBB) - popcount(blackBB));

        uint64_t tmp = whiteBB;
        while(tmp) {
            int sq = __builtin_ctzll(tmp);
            score += PIECE_SQUARE_TABLE[piecetype][flip(sq)];
            tmp &= tmp - 1;
        }

        tmp = blackBB;
        while(tmp) {
            int sq = __builtin_ctzll(tmp);
            score -= PIECE_SQUARE_TABLE[piecetype][flip(sq)];
            tmp &= tmp - 1;
        }
    }

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

int Eval::minimax(int depth, Move *best_move) {
    if(depth == 0) return evaluate();
    if(node_count >= MAX_NODES) return evaluate();  
    
    node_count++;

    std::vector<Move> moves;
    moves.reserve(80); 
    
    bool side = bitboard.getMovingSide();
    bool isInCheck = bitboard.isKingInCheck(side);

    movegen.generateMoves(bitboard, moves);

    if(moves.empty()) {
        if(isInCheck) {
            return side == WHITE ? -INF : INF;
        }

        return 0;  
    }
 
    if(side == WHITE) {
        int best = -INF;
        
        for(const Move &move : moves) {
            bitboard.makeMove(move);
            int score = minimax(depth - 1, nullptr);
            bitboard.undoMove();
            
            if(score >= best) {
                best = score;
                if(best_move) *best_move = move;
            }
        }

        return best;
    }

    int best = INF;

    for(const Move &move : moves) {
         bitboard.makeMove(move);
         int score = minimax(depth - 1, nullptr);
         bitboard.undoMove();

         if(score <= best) {
            best = score;
            if(best_move) *best_move = move;
         }
    }

    return best;
}

Move Eval::search(int search_depth) {
    Move best = Move(0, 0, QUIET);
    node_count = 0;  
    minimax(search_depth, &best);
    return best;
}