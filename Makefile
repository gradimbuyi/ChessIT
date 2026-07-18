CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Iinclude
DEBUG_FLAGS := -std=c++17 -O0 -g -Wall -Iinclude
GTEST_DIR := /opt/homebrew/opt/googletest
GTEST_FLAGS := -I$(GTEST_DIR)/include -L$(GTEST_DIR)/lib -lgtest -lgtest_main -pthread

TARGET := build/engine
TEST_TARGET := build/tests

SRC := $(wildcard src/*.cpp)
ENGINE_SRC := $(filter-out src/main.cpp,$(SRC))
TEST_SRC := $(wildcard tests/*.cpp)
OBJ := $(patsubst src/%.cpp,build/%.o,$(SRC))
ENGINE_OBJ := $(patsubst src/%.cpp,build/%.o,$(ENGINE_SRC))
TEST_OBJ := $(patsubst tests/%.cpp,build/tests/%.o,$(TEST_SRC))

all: $(TARGET)

$(TARGET): $(OBJ)
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -o $@ $^

build/%.o: src/%.cpp
	@mkdir -p build
	$(CXX) $(CXXFLAGS) -c $< -o $@

build/tests/%.o: tests/%.cpp
	@mkdir -p build/tests
	$(CXX) $(CXXFLAGS) -c $< -o $@

test: $(ENGINE_OBJ) $(TEST_OBJ)
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) $^ $(GTEST_FLAGS)
	./$(TEST_TARGET)

debug: CXXFLAGS=$(DEBUG_FLAGS)
debug: clean all

run: all
	./$(TARGET)

clean:
	rm -rf build

.PHONY: all test debug run clean