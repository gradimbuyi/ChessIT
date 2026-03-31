CXX         = g++
CXXFLAGS    = -std=c++17 -O2 -Wall
DEBUG_FLAGS = -std=c++17 -O0 -g -Wall
GTEST_DIR   = /opt/homebrew/opt/googletest
GTEST_FLAGS = -I$(GTEST_DIR)/include -L$(GTEST_DIR)/lib -lgtest -lgtest_main -pthread

TARGET      = build/engine
TEST_TARGET = build/test
SRCS        = src/main.cpp src/bitboard.cpp src/movegen.cpp src/perft.cpp src/console.cpp src/ucicomm.cpp
TEST_SRCS   = tests/perft_tests.cpp src/bitboard.cpp src/movegen.cpp src/perft.cpp

OBJS        = $(patsubst src/%.cpp, build/%.o, $(SRCS))
TEST_OBJS   = $(patsubst %.cpp, build/%.o, $(TEST_SRCS))

all: build $(TARGET)

build:
	mkdir -p build build/tests

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

build/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: build $(TEST_TARGET)
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