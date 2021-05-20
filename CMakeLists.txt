cmake_minimum_required(VERSION 3.17)
project(TicTacToe)

set(CMAKE_CXX_STANDARD 20)

add_executable(TicTacToe main.cpp Player.h myconcepts.h TicTacField.cpp TicTacField.h application.h commons.cpp commons.h heuristics.cpp TicTacFieldDefinitions.h Line.h heuristicsweights.h Heuristics.h Line.cpp HeuristicExtremeValues.cpp HeuristicExtremeValues.h)