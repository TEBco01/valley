/*
Copyright 2019 Tyler Behme

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#pragma once

#include <iostream>
#include <moves.h>
#include <bitboard.h>
#include <game.h>
#include <moves.h>
#include <moveGeneration.h>

U64 Perft(int depth, game Game);
std::string moveToAlgebraic(const move change);
move algebraicToMove(const char* moveString);
void generateConstBitboard();
int rangeRandomAlg2 (int min, int max);
void printArray(char array[64]);
void printMovePossibilities(const bitboards originalBoard, const moveList possibleMoves);
