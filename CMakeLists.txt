cmake_minimum_required(VERSION 3.17)
project(chess C)

set(CMAKE_C_STANDARD 99)

add_executable(chess main.c chess.c ai.c a.h move_positon_evaluation.c)