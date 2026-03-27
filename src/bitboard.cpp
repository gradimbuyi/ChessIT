#include <iostream>
#include "bitboard.h"

Bitboard::Bitboard() { 
    piecesBB[WHITE][PAWNS]   = 0x000000000000FF00ULL;
    piecesBB[WHITE][KNIGHTS] = 0x0000000000000042ULL;
    piecesBB[WHITE][BISHOPS] = 0x0000000000000024ULL;
    piecesBB[WHITE][ROOKS]   = 0x0000000000000081ULL;
    piecesBB[WHITE][QUEENS]  = 0x0000000000000008ULL;
    piecesBB[WHITE][KING]    = 0x0000000000000010ULL;
    piecesBB[BLACK][PAWNS]   = 0x00FF000000000000ULL;
    piecesBB[BLACK][KNIGHTS] = 0x4200000000000000ULL;
    piecesBB[BLACK][BISHOPS] = 0x2400000000000000ULL;    
    piecesBB[BLACK][ROOKS]   = 0x8100000000000000ULL;
    piecesBB[BLACK][QUEENS]  = 0x0800000000000000ULL;
    piecesBB[BLACK][KING]    = 0x1000000000000000ULL;

    updateOccupancy();

    side = WHITE;
    ep_square = -1;

    castling.king_side[WHITE]  = true;
    castling.king_side[BLACK]  = true;
    castling.queen_side[WHITE] = true;
    castling.queen_side[BLACK] = true;
}

void Bitboard::printBitboard(int color, int type) {
    uint64_t bb = getPieces(color, type);

    if((color > 1 || color < 0) && (type > 5 || type < 0)) {
        std::cout << "Invalid inputs. Could not print out bitboard" << std::endl << std::endl;
        return ;
    }

    std::cout << "Priting bitboard..." << std::endl << std::endl;

    for(int rank = 7; rank >= 0; rank--) {
        std::cout << rank + 1 << " ";
        for(int file = 0; file < 8; file++) {
            int sq = rank * 8 + file;
            if ((bb >> sq) & 1) {
                std::cout << "1 ";
            } else {
                std::cout << "0 ";
            }
        }
        std::cout << "\n";
    }

    std::cout << "\n   a b c d e f g h\n\n";
}

void Bitboard::visualizeBoard() {
    std::cout << "Printing Chess Board..." << std::endl << std::endl;

    for(int rank = 7; rank >= 0; rank--) {
        std::cout << rank + 1 << "  ";
        for(int file = 0; file < 8; file++) {
            int sq = rank * 8 + file;
            
            uint64_t bit = 1ULL << sq;
            
            char p = '.';

            uint64_t (*bb)[6] = piecesBB;
            
                 if(bb[WHITE][PAWNS]   & bit) p = 'P';
            else if(bb[WHITE][KNIGHTS] & bit) p = 'N';
            else if(bb[WHITE][BISHOPS] & bit) p = 'B';
            else if(bb[WHITE][ROOKS]   & bit) p = 'R';
            else if(bb[WHITE][QUEENS]  & bit) p = 'Q';
            else if(bb[WHITE][KING]    & bit) p = 'K';
            else if(bb[BLACK][PAWNS]   & bit) p = 'p';
            else if(bb[BLACK][KNIGHTS] & bit) p = 'n';
            else if(bb[BLACK][BISHOPS] & bit) p = 'b';
            else if(bb[BLACK][ROOKS]   & bit) p = 'r';
            else if(bb[BLACK][QUEENS]  & bit) p = 'q';
            else if(bb[BLACK][KING]    & bit) p = 'k';

            std::cout << p << " ";
        }
        std::cout << std::endl;
    }
    std::cout <<  std::endl << "   a b c d e f g h" << std::endl << std::endl;
}

int Bitboard::getPieceType(int color, int square) {
    uint64_t mask = 1ULL << square;

    for(int piece_type = 0; piece_type < 6; piece_type++) {
        if(piecesBB[color][piece_type] & mask) return piece_type;
    }

    return -1;
}

void Bitboard::updateOccupancy() {
    white_occupied = 0;
    black_occupied = 0;

    for(int piece_type = 0; piece_type < 6; piece_type++) {
        white_occupied |= piecesBB[WHITE][piece_type];
        black_occupied |= piecesBB[BLACK][piece_type];
    }

    occupied = white_occupied | black_occupied;
}

void Bitboard::saveCurrentState() {
    BoardState state;

    for(int color = 0; color < 2; color++) {
        for(int piece = 0; piece < 6; piece++) {
            state.pieces[color][piece] = piecesBB[color][piece];
        }
    }

    state.side      = side;
    state.ep_square = ep_square;
    state.castling  = castling;

    history.push_back(state);
}

void Bitboard::specialMoveHandler(int from, int to, int flag) {
    bool enemy = !side;

    switch(flag) {
         case DOUBLE_PAWN_PUSH: {
            ep_square = (from + to) / 2;
            break;
        }

        case EN_CAPTURES: {
            int square = side == WHITE ? to - 8 : to + 8;
            piecesBB[enemy][PAWNS] &= -(1ULL << square); 
            break;
        }

        case KING_CASTLE: {
            int rook_from = side == WHITE ? 7 : 63;
            int rook_to   = side == WHITE ? 5 : 61;
            
            piecesBB[enemy][ROOKS]  &= -(1ULL << rook_from);
            piecesBB[enemy][ROOKS]  |=  (1ULL << rook_to);
            castling.king_side[side] =  false; 
            break;
        }

        case QUEEN_CASTLE: {
            int rook_from = side == WHITE ? 0 : 56;
            int rook_to   = side == WHITE ? 3 : 59;
            
            piecesBB[enemy][ROOKS]   &= -(1ULL << rook_from);
            piecesBB[enemy][ROOKS]   |=  (1ULL << rook_to);
            castling.queen_side[side] =  false; 
            break;
        }

        default:
            break;
    }
}

void Bitboard::nonEnPassantCaptureHanlder(int to_mask) {
    bool enemy = !side;
    
    for(int pt = 0; pt < 6; pt++) {
        if(piecesBB[enemy][pt] & to_mask) {
            piecesBB[enemy][pt] &= ~to_mask;
        }
    }
}

void Bitboard::promoMoveHandler(int to_mask, int flag) {
    switch(flag) {
        case KNIGHT_PROMOTION:
        case KNIGHT_PROMO_CAPTURE: {
            piecesBB[side][KNIGHTS] |= to_mask;
            break;
        }

        case BISHOP_PROMOTION:
        case BISHOP_PROMO_CAPTURE: {
            piecesBB[side][BISHOPS] |= to_mask;
            break;
        }
          
        case ROOK_PROMOTION:
        case ROOK_PROMO_CAPTURE: {
            piecesBB[side][ROOKS] |= to_mask;
            break;
        }

        case QUEEN_PROMOTION:
        case QUEEN_PROMO_CAPTURE: {
            piecesBB[side][QUEENS] |= to_mask;
            break;
        }

        default:
            break;
    }
}

void Bitboard::revokeCastlingRights() {
    castling.king_side[side]  = false;
    castling.queen_side[side] = false;
}

void Bitboard::revokeRookSideCastlingRight(int color, int from) {
    if(color == WHITE) {
        if(from == 7)  castling.king_side[color]  = false;
        if(from == 0)  castling.queen_side[color] = false;
    } else {
        if(from == 63) castling.king_side[color]  = false;
        if(from == 56) castling.queen_side[color] = false;
    }
}

void Bitboard::makeMove(const Move &move) {
    saveCurrentState();

    int from   = move.getFrom();
    int to     = move.getTo();
    int flag   = move.getFlag();
    bool enemy = !side;

    uint64_t from_mask = 1ULL << from;
    uint64_t to_mask   = 1ULL << to;

    int piece = getPieceType(side, from);

    ep_square = -1;

    piecesBB[side][piece] &= ~from_mask;

    specialMoveHandler(from, to, flag);
    
    if (move.isCapture() && flag != EN_CAPTURES) nonEnPassantCaptureHanlder(to_mask);
    
    promoMoveHandler(to_mask, flag);
    
    if (piece == KING)    revokeCastlingRights();
    if (piece == ROOKS)   revokeRookSideCastlingRight(side, from);
    if (move.isCapture()) revokeRookSideCastlingRight(enemy, to);

    updateOccupancy();
    
    side = enemy;
}

void Bitboard::undoMove() {
    BoardState state = history.back();
    history.pop_back();

    for(int color = 0; color < 2; color++) {
        for(int piece = 0; piece < 6; piece++) {
            piecesBB[color][piece] = state.pieces[color][piece];
        }
    }

    side = state.side;
    ep_square = state.ep_square;
    castling = state.castling;

    updateOccupancy();
}