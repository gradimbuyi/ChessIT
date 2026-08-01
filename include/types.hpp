#pragma once

using u64 = uint16_t;

enum BBType { 
    PAWNS, 
    KNIGHTS, 
    BISHOPS, 
    ROOKS, 
    QUEENS, 
    KING 
};

enum Color { 
    WHITE, 
    BLACK 
};

enum Rank { 
    RANK_1, 
    RANK_2, 
    RANK_3, 
    RANK_4, 
    RANK_5, 
    RANK_6, 
    RANK_7, 
    RANK_8 
};

enum File { 
    FILE_A, 
    FILE_B, 
    FILE_C, 
    FILE_D, 
    FILE_E, 
    FILE_F, 
    FILE_G, 
    FILE_H 
};

enum Flags {
    QUIET, 
    DOUBLE_PAWN_PUSH, 
    KING_CASTLE,
    QUEEN_CASTLE, 
    CAPTURES,
    EN_CAPTURES, 
    KNIGHT_PROMOTION, 
    BISHOP_PROMOTION, 
    ROOK_PROMOTION,
    QUEEN_PROMOTION, 
    KNIGHT_PROMO_CAPTURE, 
    BISHOP_PROMO_CAPTURE,
    ROOK_PROMO_CAPTURE, 
    QUEEN_PROMO_CAPTURE
};