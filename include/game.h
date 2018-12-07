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

#pragma once

#include <bitboard.h>
#include <moves.h>

struct game {
  bitboards boards;
  moveList history;
  bool blacksTurn = 0;

  game() {
    boards.initStandardBoard();
  }
  game(bitboards newBoards) {
    boards = newBoards;
  }
  void makeMove(move moveMade) {
    applyMove(&boards, moveMade);
    history.addMove(moveMade);
    blacksTurn ^= 1;
  }
  void undoMove(/*move moveUnmade*/) {
    move moveUnmade = history.moves[history.length - 1];
    unapplyMove(&boards, moveUnmade);
    history.removeMove(history.length - 1);
    blacksTurn ^= 1;
  }

  moveList generateLegalMoves() {
    if(blacksTurn) {
      return possibleMovesB(history, boards);
    } else {
      return possibleMovesW(history, boards);
    }
  }
};
