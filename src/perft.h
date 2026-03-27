#ifndef PERFT_H
#define PERFT_H

#include "bitboard.h"
#include "movegen.h"

long perft(Bitboard &bitboard, MoveGenerator &movegen, int depth);

#endif