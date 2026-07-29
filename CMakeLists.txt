cmake_minimum_required(VERSION 3.15)

project(ChessIT LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES "src/*.cpp")

add_executable(engine ${SOURCES})
target_include_directories(engine PRIVATE include)