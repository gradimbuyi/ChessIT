#include <iostream>
#include "bitboard.h"

Bitboard::Bitboard() { 
    initializeSTARTPOSpiecesBB();

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

    if((color > 1 || color < 0) || (type > 5 || type < 0)) {
        std::cout << "Invalid inputs. Could not print out bitboard" << std::endl << std::endl;
        return ;
    }

    std::cout << "Printing bitboard..." << std::endl << std::endl;

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

void Bitboard::initializeSTARTPOSpiecesBB() {
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
            piecesBB[enemy][PAWNS] &= ~(1ULL << square); 
            break;
        }

        case KING_CASTLE: {
            int rook_from = side == WHITE ? 7 : 63;
            int rook_to   = side == WHITE ? 5 : 61;
            
            piecesBB[side][ROOKS]  &= ~(1ULL << rook_from);
            piecesBB[side][ROOKS]  |=  (1ULL << rook_to);
            castling.king_side[side] =  false; 
            break;
        }

        case QUEEN_CASTLE: {
            int rook_from = side == WHITE ? 0 : 56;
            int rook_to   = side == WHITE ? 3 : 59;
            
            piecesBB[side][ROOKS]   &= ~(1ULL << rook_from);
            piecesBB[side][ROOKS]   |=  (1ULL << rook_to);
            castling.queen_side[side] =  false; 
            break;
        }

        default:
            break;
    }
}

void Bitboard::nonEnPassantCaptureHanlder(uint64_t to_mask) {
    bool enemy = !side;
    
    for(int pt = 0; pt < 6; pt++) {
        if(piecesBB[enemy][pt] & to_mask) {
            piecesBB[enemy][pt] &= ~to_mask;
            break;
        }
    }
}

void Bitboard::promoMoveHandler(uint64_t to_mask, int flag) {
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

    int  from  = move.getFrom();
    int  to    = move.getTo();
    int  flag  = move.getFlag();
    bool enemy = !side;

    uint64_t from_mask = 1ULL << from;
    uint64_t to_mask   = 1ULL << to;

    int piece = getPieceType(side, from);

    if(piece == -1) {
        std::cerr << "Invalid piece on square " << from << std::endl;
        std::abort();
    }

    ep_square = -1;

    piecesBB[side][piece] &= ~from_mask;

    specialMoveHandler(from, to, flag);
    
    if (move.isCapture() && flag != EN_CAPTURES) nonEnPassantCaptureHanlder(to_mask);
    
    promoMoveHandler(to_mask, flag);

    if (flag < KNIGHT_PROMOTION) piecesBB[side][piece] |= to_mask;
    if (piece == KING)           revokeCastlingRights();
    if (piece == ROOKS)          revokeRookSideCastlingRight(side, from);
    
    if (move.isCapture()) {
        int captured = getPieceType(enemy, to);
        
        if(captured == ROOKS) {
            revokeRookSideCastlingRight(enemy, to);
        }
    }

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

bool Bitboard::isSquareAttacked(int square, int color) const {
    uint64_t enemy_pawns   = piecesBB[color][PAWNS];
    uint64_t enemy_rooks   = piecesBB[color][ROOKS];
    uint64_t enemy_bishops = piecesBB[color][BISHOPS];
    uint64_t enemy_knights = piecesBB[color][KNIGHTS];
    uint64_t enemy_queens  = piecesBB[color][QUEENS]; 
    uint64_t enemy_king    = piecesBB[color][KING];

    int sq_rank = square >> 3;
    int sq_file = square &  7;

    if(color == WHITE) {
        if((sq_file > 0 && square - 9 >= 0) && (enemy_pawns & (1ULL << (square - 9)))) return true;
        if((sq_file < 7 && square - 7 >= 0) && (enemy_pawns & (1ULL << (square - 7)))) return true;

    } else {
        if((sq_file > 0 && square + 7 < 64) && (enemy_pawns & (1ULL << (square + 7)))) return true;
        if((sq_file < 7 && square + 9 < 64) && (enemy_pawns & (1ULL << (square + 9)))) return true;
    }

    int knight_offsets[] = { 6, 10, 15, 17, -6, -10, -15, -17 };

    for(int i = 0; i < 8; i++) {
        int to = knight_offsets[i] + square;

        if(to < 0 || to > 63) continue;

        int file_diff = abs((to & 7)  - sq_file);
        int rank_diff = abs((to >> 3) - sq_rank);

        if(!((file_diff == 1 && rank_diff == 2) || (file_diff == 2 && rank_diff == 1))) continue;

        if(enemy_knights & (1ULL << to)) return true;
    }

    int king_offsets[] = { 1, 7, 8, 9, -1, -7, -8, -9 };
    
    for(int offset : king_offsets) {
        int to = square + offset;
    
        if(to < 0 || to > 63) continue;

        int to_rank = to >> 3;
        int to_file = to & 7;
    
        if(abs(to_rank - sq_rank) > 1 || abs(to_file - sq_file) > 1) continue;

        if(enemy_king & (1ULL << to)) return true;
    }

    auto slidingAttacked = [&](uint64_t attackers, const int directions[], int directions_count) -> bool {
        for(int i = 0; i < directions_count; i++) {
            int direction = directions[i];
            int curr_sq   = square;
            int curr_rank = sq_rank;
            int curr_file = sq_file;

            while(true) {
                int prev_rank = curr_rank;
                int prev_file = curr_file;

                curr_sq += direction;

                if(curr_sq < 0 || curr_sq > 63) break;

                curr_rank = curr_sq >> 3;
                curr_file = curr_sq &  7;

                if(abs(curr_rank - prev_rank ) > 1 || abs(curr_file - prev_file) > 1) break;

                uint64_t curr_sq_mask = 1ULL << curr_sq; 

                if(attackers & curr_sq_mask) return true;
                if(occupied  & curr_sq_mask) break;
            }
        }

        return false;
    };

    int      orthogonal[]         = { 1, -1, 8, -8 };
    uint64_t orthogonal_attackers = enemy_rooks | enemy_queens;

    if(slidingAttacked(orthogonal_attackers, orthogonal, 4)) return true;

    int      diagonal[]         = { 7, 9, -7, -9 };
    uint64_t diagonal_attackers = enemy_bishops | enemy_queens;

    if(slidingAttacked(diagonal_attackers, diagonal, 4)) return true;

    return false;
}

bool Bitboard::isKingInCheck(int color) const {
    int king_sq = __builtin_ctzll(piecesBB[color][KING]);
    return isSquareAttacked(king_sq, !color);
}

void Bitboard::resetState() {
    for(int color = 0; color < 2; color++) {
        for(int piece = 0; piece < 6; piece++) {
            piecesBB[color][piece] = 0ULL;
        }
    }

    ep_square = -1;

    castling.king_side[WHITE]  = false;
    castling.king_side[BLACK]  = false;
    castling.queen_side[WHITE] = false;
    castling.queen_side[BLACK] = false;

    updateOccupancy();

    history.clear();
}

void Bitboard::loadFEN(const std::string &fen) {
    resetState();

    int i    = 0;
    int rank = 7;
    int file = 0;

    while(fen[i] != ' ') {
        char curr_character = fen[i++];

        if(curr_character == '/') {
            rank--;
            file = 0;
            continue;
        }

        if(curr_character >= '1' && curr_character <= '8') {
            file += (curr_character - '0');
            continue;
        }

        int square = rank * 8 + file;
        int color  = (curr_character >= 'a' && curr_character <= 'z') ? BLACK : WHITE;

        switch(curr_character) {
            case 'P' : case 'p' : piecesBB[color][PAWNS]   |= (1ULL << square); break;
            case 'N' : case 'n' : piecesBB[color][KNIGHTS] |= (1ULL << square); break;
            case 'B' : case 'b' : piecesBB[color][BISHOPS] |= (1ULL << square); break;
            case 'R' : case 'r' : piecesBB[color][ROOKS]   |= (1ULL << square); break;
            case 'Q' : case 'q' : piecesBB[color][QUEENS]  |= (1ULL << square); break;
            case 'K' : case 'k' : piecesBB[color][KING]    |= (1ULL << square); break;
        }

        file++;
    }

    i++;

    side = (fen[i] == 'w') ? WHITE : BLACK;
    i += 2;

    if(fen[i] == '-') {
        i++;

    } else {
        while(fen[i] != ' ') {
            switch(fen[i]) {
                case 'K': castling.king_side[WHITE]  = true; break;
                case 'Q': castling.queen_side[WHITE] = true; break;
                case 'k': castling.king_side[BLACK]  = true; break;
                case 'q': castling.queen_side[BLACK] = true; break;
            }
            i++;
        }
    }

    i++;

    if(fen[i] == '-') {
        ep_square = -1;
        i++;

    } else {
        int ep_file = fen[i]     - 'a';
        int ep_rank = fen[i + 1] - '1';
        ep_square = ep_rank * 8 + ep_file;
        i += 2;
    }

    updateOccupancy();
}

int Bitboard::getPieceTypePublic(int color, int square) { 
    return getPieceType(color, square); 
}
 
void Bitboard::updateOccupancyPublic() { 
    updateOccupancy(); 
}
void Bitboard::tempRemovePiece(int color, int piece, uint64_t mask) { 
    piecesBB[color][piece] &= ~mask;
    updateOccupancy();
}

void Bitboard::tempRestorePiece(int color, int piece, uint64_t mask) {
    piecesBB[color][piece] |= mask;
    updateOccupancy();
}