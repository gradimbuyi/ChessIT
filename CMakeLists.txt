cmake_minimum_required(VERSION 3.15)

project(ChessIT LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB ENGINE_SOURCES "src/*.cpp")
file(GLOB ARENA_SOURCES  "arena/src/*.cpp")

add_executable(engine ${ENGINE_SOURCES})
target_include_directories(engine PRIVATE include)

add_executable(arena ${ARENA_SOURCES})
target_include_directories(arena PRIVATE include arena/include)