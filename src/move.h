#ifndef MOVE_H
#define MOVE_H

enum Flags {
    QUIET, DOUBLE_PAWN_PUSH, KING_CASTLE, QUEEN_CASTLE, CAPTURES,
    EN_CAPTURES, KNIGHT_PROMOTION, BISHOP_PROMOTION, ROOK_PROMOTION,
    QUEEN_PROMOTION, KNIGHT_PROMO_CAPTURE, BISHOP_PROMO_CAPTURE,
    ROOK_PROMO_CAPTURE, QUEEN_PROMO_CAPTURE
};

class Move {
    protected:
        unsigned int m_Move;

    Move(unsigned int from, unsigned int to, unsigned int flags) {
        m_Move = ((flags & 0xf) << 12) | ((from & 0x3f) << 6) | (to & 0x3f);
    }

    unsigned int getTo()   { return m_Move & 0x3f; }
    unsigned int getFrom() { return (m_Move >> 6) & 0x3f; }
    unsigned int getFlag() { return (m_Move >> 12) & 0x0f; }

    void setTo(unsigned int to) { m_Move &= ~0x3f; m_Move |= to & 0x3f; }
    void setFrom(unsigned int from) { m_Move &= ~0xfc0; m_Move |= (from & 0x3f) << 6; }
    bool isCapture() const { return (m_Move & CAPTURES) != 0; }
    
    unsigned int getButterflyIndex() const { return m_Move & 0x0fff; } 

    bool operator==(Move a) const { return (m_Move & 0xffff) == (a.m_Move & 0xffff); }
    bool operator!=(Move a) const { return (m_Move & 0xffff) != (a.m_Move & 0xffff); }
    void operator=(Move a) { m_Move = a.m_Move; }    
};

#endif