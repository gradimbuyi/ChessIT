CXX         = g++
CXXFLAGS    = -std=c++17 -O2 -Wall
DEBUG_FLAGS = -std=c++17 -O0 -g -Wall
GTEST_DIR   = /opt/homebrew/opt/googletest
GTEST_FLAGS = -I$(GTEST_DIR)/include -L$(GTEST_DIR)/lib -lgtest -lgtest_main -pthread

TARGET      = engine/build/engine
TEST_TARGET = engine/build/test
SRCS        = engine/src/main.cpp engine/src/bitboard.cpp engine/src/movegen.cpp engine/src/perft.cpp engine/src/console.cpp engine/src/ucicomm.cpp engine/src/eval.cpp
TEST_SRCS   = engine/tests/perft_tests.cpp engine/tests/move_tests.cpp engine/tests/attack_movegen_tests.cpp engine/src/bitboard.cpp engine/src/movegen.cpp engine/src/perft.cpp engine/src/eval.cpp

OBJS        = $(patsubst engine/src/%.cpp, engine/build/%.o, $(SRCS))
TEST_OBJS   = $(patsubst %.cpp, engine/build/%.o, $(TEST_SRCS))

all: engine/build $(TARGET)

engine/build:
	mkdir -p engine/build engine/build/tests

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

engine/build/%.o: engine/src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: engine/build $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_SRCS)
	$(CXX) $(CXXFLAGS) $(GTEST_FLAGS) -o $(TEST_TARGET) $(TEST_SRCS)

debug: CXXFLAGS = $(DEBUG_FLAGS)
debug: clean all

run: all
	./$(TARGET)

clean:
	rm -rf build/

.PHONY: all debug run clean test