/*
Copyright 2018 Tyler Behme

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

#include <bitboard.h>

typedef unsigned char byte;

struct move {
  byte start = 0, end = 0, special = 0; // For special: 1 is castle, 2 is pawn into Queen, 3 is pawn into Rook, 4 is pawn into Knight, 5 is pawn into Bishop
};

struct moveList {
  move* moves;
  int length = 0;

  moveList() {
    moves = new move[0];
  }

  void addMove(move newMove) {
    length++;
    move* temp = moves;
    moves = new move[length];
    for(int i = 0; i < (length - 1); i++) {
      moves[i] = temp[i];
    }
    moves[length - 1] = newMove;
    delete [] temp;
  }
};

moveList possibleMovesArrayW(moveList history, char game[64]);
