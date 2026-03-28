CXX 			= g++
CXXFLAGS 		= -std=c++17 -O2 -Wall
DEBUG_FLAGS 	= -std=c++17 -OO -g -Wall
TARGET 			= build/chess
SRCS 			= src/main.cpp src/bitboard.cpp src/movegen.cpp src/perft.cpp src/console.cpp
OBJS            = $(patsubst src/%.cpp, build/%.o, $(SRCS))

all: build $(TARGET)

build: 
	mkdir -p build

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

build/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

debug: CXXFLAGS = $(DEBUG_FLAGS)
debug: clean all

run: all
	./$(TARGET)

clean:
	rm -rf build/

.PHONY: all debug run clean