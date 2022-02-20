cmake_minimum_required(VERSION 3.21)
project(chess C)

set(CMAKE_C_STANDARD 99)

add_executable(chess main.c a.h ai.c chess.c move_positon_evaluation.c)
