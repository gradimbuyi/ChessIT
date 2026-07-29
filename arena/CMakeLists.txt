cmake_minimum_required(VERSION 3.15)

project(Arena LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES "src/*.cpp")

add_executable(arena ${SOURCES})
target_include_directories(arena PRIVATE include)