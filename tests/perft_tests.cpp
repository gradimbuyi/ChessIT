#include <gtest/gtest.h>

#include "../include/bitboard.hpp"
#include "../include/movegen.hpp"
#include "../include/perft.hpp"

class PerftTest : public ::testing::Test {
    protected:
        Bitboard bitboard;
        Perft perft;
};


TEST_F(PerftTest, StartPositionDepth0) {
    bitboard.loadFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    PerftStats stats = perft.perfStats(bitboard, 0);

    EXPECT_EQ(stats.nodes,       1ULL);
    EXPECT_EQ(stats.captures,    0ULL);
    EXPECT_EQ(stats.en_passants, 0ULL);
    EXPECT_EQ(stats.castles,     0ULL);
    EXPECT_EQ(stats.promotions,  0ULL);
    EXPECT_EQ(stats.checks,      0ULL);
    EXPECT_EQ(stats.checkmates,  0ULL);
}

TEST_F(PerftTest, StartPositionDepth1) {
    bitboard.loadFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    PerftStats stats = perft.perfStats(bitboard, 1);

    EXPECT_EQ(stats.nodes,       20ULL);
    EXPECT_EQ(stats.captures,    0ULL);
    EXPECT_EQ(stats.en_passants, 0ULL);
    EXPECT_EQ(stats.castles,     0ULL);
    EXPECT_EQ(stats.promotions,  0ULL);
    EXPECT_EQ(stats.checks,      0ULL);
    EXPECT_EQ(stats.checkmates,  0ULL);
}

TEST_F(PerftTest, StartPositionDepth2) {
    bitboard.loadFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    PerftStats stats = perft.perfStats(bitboard, 2);

    EXPECT_EQ(stats.nodes,       400ULL);
    EXPECT_EQ(stats.captures,    0ULL);
    EXPECT_EQ(stats.en_passants, 0ULL);
    EXPECT_EQ(stats.castles,     0ULL);
    EXPECT_EQ(stats.promotions,  0ULL);
    EXPECT_EQ(stats.checks,      0ULL);
    EXPECT_EQ(stats.checkmates,  0ULL);
}

TEST_F(PerftTest, StartPositionDepth3) {
    bitboard.loadFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    PerftStats stats = perft.perfStats(bitboard, 3);

    EXPECT_EQ(stats.nodes,       8902ULL);
    EXPECT_EQ(stats.captures,    34ULL);
    EXPECT_EQ(stats.en_passants, 0ULL);
    EXPECT_EQ(stats.castles,     0ULL);
    EXPECT_EQ(stats.promotions,  0ULL);
    EXPECT_EQ(stats.checks,      12ULL);
    EXPECT_EQ(stats.checkmates,  0ULL);
}

TEST_F(PerftTest, StartPositionDepth4) {
    bitboard.loadFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    PerftStats stats = perft.perfStats(bitboard, 4);

    EXPECT_EQ(stats.nodes,       197281ULL);
    EXPECT_EQ(stats.captures,    1576ULL);
    EXPECT_EQ(stats.en_passants, 0ULL);
    EXPECT_EQ(stats.castles,     0ULL);
    EXPECT_EQ(stats.promotions,  0ULL);
    EXPECT_EQ(stats.checks,      469ULL);
    EXPECT_EQ(stats.checkmates,  8ULL);
}

TEST_F(PerftTest, StartPositionDepth5) {
    bitboard.loadFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");

    PerftStats stats = perft.perfStats(bitboard, 5);

    EXPECT_EQ(stats.nodes,       4865609ULL);
    EXPECT_EQ(stats.captures,    82719ULL);
    EXPECT_EQ(stats.en_passants, 258ULL);
    EXPECT_EQ(stats.castles,     0ULL);
    EXPECT_EQ(stats.promotions,  0ULL);
    EXPECT_EQ(stats.checks,      27351ULL);
    EXPECT_EQ(stats.checkmates,  347ULL);
}