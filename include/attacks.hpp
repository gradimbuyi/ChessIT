#pragma once

#include <array>
#include <cstdint>
#include "types.hpp"

namespace AttackTables {
    constexpr u64 makePawnAttacks(int co, int sq) {
        u64 att = 0;
        
        const int fi = sq & 7;

        if(co == WHITE) {
            if(fi > FILE_A && sq >= 9) 
                att |= 1ULL << (sq - 9);
            if(fi < FILE_H && sq >= 7) 
                att |= 1ULL << (sq - 7);
            
            return att;
        } 

        if(fi > FILE_A && sq <= 55) 
            att |= 1ULL << (sq + 7);
        if(fi < FILE_H && sq <= 54) 
            att |= 1ULL << (sq + 9);
     
        return att;
    }

    template <size_t N>
    constexpr u64 makeLeaperAttacks(
        int sq, 
        const std::array<int, N> &dr,
        const std::array<int, N> &df) 
    {
              u64 att  = 0;
        const int rank = sq >> 3;
        const int file = sq &  7;

        for(size_t i = 0; i < N; ++i) {
            const int r = rank + dr[i];
            const int f = file + df[i];

            if(r >= RANK_1 && r <= RANK_8 &&
               f >= FILE_A && f <= FILE_H)
               att |= 1ULL << (r * 8 + f);
        }

        return att;
    }

    template <size_t N>
    u64 computeSliderAttacks(
        int sq,
        u64 occupied,
        const std::array<int, N> &dr,
        const std::array<int, N> &df)
    {
              u64 att  = 0;
        const int rank = sq >> 3;
        const int file = sq &  7;
    
        for(size_t dir = 0; dir < N; ++dir) {
            int r = rank + dr[dir];
            int f = file + df[dir];

            while(r >= 0 && r < 8 && f >= 0 && f < 8) {
                const int to   = r * 8 + f;
                const u64 mask = 1ULL << to;

                att |= mask;

                if(occupied & mask)
                    break;

                r += dr[dir];
                f += dr[dir];
            }
        }

        return att;
    }

    constexpr u64 sliderAttacks(int sq, u64 occupied, BBType pt) {
        switch(pt) {
            case ROOKS:   return computeSliderAttacks(sq, occupied, ROOK_DR, ROOK_DF);
            case BISHOPS: return computeSliderAttacks(sq, occupied, BISHOP_DR, BISHOP_DF);
            default:      return 0; 
        }
    }

    template <size_t N>
    constexpr auto makeTable(
        const std::array<int, N> &dr,
        const std::array<int, N> &df) 
    {
        std::array<uint64_t, 64> table{};
        
        for(int sq = 0; sq < 64; ++sq) 
            table[sq] = makeLeaperAttacks(sq, dr, df);

        return table;
    }

    template <typename AttackFn> 
    constexpr auto makeColoredTable(AttackFn fn) {
        std::array<std::array<uint64_t, 64>, 2> table{};

        for(int co = WHITE; co <= BLACK; ++co)
            for(int sq = 0; sq < 64; ++sq)
                table[co][sq] = fn(co, sq);

        return table;
    }

    inline constexpr std::array<int, 8> KNIGHT_DR = { -2, -2, -1, -1,  1,  1,  2, 2 };
    inline constexpr std::array<int, 8> KNIGHT_DF = { -1,  1, -2,  2, -2,  2, -1, 1 };
    inline constexpr std::array<int, 8> KING_DR   = { -1, -1, -1,  0,  0,  1,  1, 1 };
    inline constexpr std::array<int, 8> KING_DF   = { -1,  0,  1, -1,  1, -1,  0, 1 };
    inline constexpr std::array<int, 4> ROOK_DR   = {  1, -1,  0,  0 };
    inline constexpr std::array<int, 4> ROOK_DF   = {  0,  0,  1, -1 };
    inline constexpr std::array<int, 4> BISHOP_DR = {  1,  1, -1, -1 };
    inline constexpr std::array<int, 4> BISHOP_DF = {  1, -1,  1, -1 };

    // Precomputed attack tables
    
    inline constexpr auto PAWN_ATTACK_TABLE   = makeColoredTable(makePawnAttacks);
    inline constexpr auto KNIGHT_ATTACK_TABLE = makeTable(KNIGHT_DR, KNIGHT_DF);
    inline constexpr auto KING_ATTACK_TABLE   = makeTable(KING_DR, KING_DF);
}