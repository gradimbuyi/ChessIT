#ifndef CHESS_BOARD
#define CHESS_BOARD

#include <cstdint> 

#define NOT_A_FILE 0xFEFEFEFEFEFEFEFEULL
#define NOT_H_FILE 0x7F7F7F7F7F7F7F7FULL
#define WHITE_RANK_4 0x00000000FF000000ULL
#define BLACK_RANK_4 0x000000FF00000000ULL
#define WHITE_PAWNS_START_RANK 0x000000000000FF00ULL
#define BLACK_PAWNS_START_RANK 0x00FF000000000000ULL
#define WHITE_PROMOTION_RANK 0xFF00000000000000ULL
#define BLACK_PROMOTION_RANK 0x00000000000000FFULL

struct Move {
    int moving_from;
    int moving_to;
    int piece;
    int captured;
    int promoted;
    bool castling;
    bool enpasant;
};

enum { NONE, PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING };

enum enumSquare {
    a1, b1, c1, d1, e1, f1, g1, h1,
    a2, b2, c2, d2, e2, f2, g2, h2,
    a3, b3, c3, d3, e3, f3, g3, h3,
    a4, b4, c4, d4, e4, f4, g4, h4,
    a5, b5, c5, d5, e5, f5, g5, h5,
    a6, b6, c6, d6, e6, f6, g6, h6,
    a7, b7, c7, d7, e7, f7, g7, h7,
    a8, b8, c8, d8, e8, f8, g8, h8
};

class ChessBoard {
    private:
        uint64_t white_pawns;
        uint64_t white_knights;
        uint64_t white_bishops;
        uint64_t white_rooks;
        uint64_t white_queens;
        uint64_t white_king;
        uint64_t black_pawns;
        uint64_t black_knights;
        uint64_t black_bishops;
        uint64_t black_rooks;
        uint64_t black_queens;
        uint64_t black_king;
        uint64_t occupied;
        uint64_t white_occupied;
        uint64_t black_occupied;
        uint64_t en_passant_square;
        bool white_to_move; 
        bool white_king_side_castle;
        bool white_queen_side_castle;
        bool black_king_side_castle;
        bool black_queen_side_castle;

    public:
        ChessBoard();
        void printBitboard(bool is_white, int piece);
        void visualizeBoard();
        void changeTurn() { white_to_move = !white_to_move; }
        uint64_t getWhitePawns()        const { return white_pawns; }
        uint64_t getWhiteKnights()      const { return white_knights; }
        uint64_t getWhiteBishops()      const { return white_bishops; }
        uint64_t getWhiteRooks()        const { return white_rooks; }
        uint64_t getWhiteQueens()       const { return white_queens; }
        uint64_t getWhiteKing()         const { return white_king; }
        uint64_t getBlackPawns()        const { return black_pawns; }
        uint64_t getBlackKnights()      const { return black_knights; }
        uint64_t getBlackBishops()      const { return black_bishops; }
        uint64_t getBlackRooks()        const { return black_rooks; }
        uint64_t getBlackQueens()       const { return black_queens; }
        uint64_t getBlackKing()         const { return black_king; }
        uint64_t getOccupied()          const { return occupied; }
        uint64_t getWhiteOccupied()     const { return white_occupied; } 
        uint64_t getBlackOccupied()     const { return black_occupied; }
        uint64_t getEnPassantSquare()   const { return en_passant_square; }
        bool isWhiteTurn()              const { return white_to_move; }
        bool isWhiteKingSideCastle()    const { return white_king_side_castle; }
        bool isWhiteQueenSideCastle()   const { return white_queen_side_castle; }
        bool isBlackKingSideCastle()    const { return black_king_side_castle; }
        bool isBlackQueenSideCastle()   const { return black_queen_side_castle; }
};

#endif