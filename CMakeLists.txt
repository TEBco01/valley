cmake_minimum_required(VERSION 3.14)
project(valley)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_executable(valley
        include/bitboard.h
        include/evaluator.h
        include/game.h
        include/moveGeneration.h
        include/moves.h
        include/uci.h
        include/utilities.h
        src/bitboard.cpp
        src/evaluator.cpp
        src/game.cpp
        src/main.cpp
        src/moveGeneration.cpp
        src/moves.cpp
        src/uci.cpp
        src/utilities.cpp)
