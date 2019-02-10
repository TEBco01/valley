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
#include <iostream> // For debugging only

struct boardNode {
  bitboards boards;
  boardNode* last;
};

class boardStack {
  boardNode* tail = NULL;

public:
  void push(bitboards boards);
  bitboards pop();
};

void boardStack::push(bitboards boards) {
  boardNode* node = new boardNode();
  node->boards = boards;
  node->last = tail;
  tail = node;
}

bitboards boardStack::pop() {
 boardNode* newTail = tail->last;
 bitboards boards = tail->boards;
 delete tail;
 tail = newTail;
 return boards;
}

struct game {
  bitboards boards;
  moveList history;
  boardStack boardHistory;
  bool blacksTurn = 0;

  game() {
    boards.initStandardBoard();
  }
  game(bitboards newBoards) {
    boards = newBoards;
  }
  void makeMove(move moveMade) {
    boardHistory.push(boards);
    applyMove(&boards, moveMade);
    history.addMove(moveMade);
    blacksTurn ^= 1;
  }
  void undoMove(/*move moveUnmade*/) {
    //move moveUnmade = history.moves[history.length - 1];
    //unapplyMove(&boards, moveUnmade);
    boards = boardHistory.pop();
    history.removeMove(history.length - 1);
    blacksTurn ^= 1;
  }

  bool isGameLegal() {
    moveList moves = generateSemilegalMoves();
    for(int i = 0; i < moves.length; i++) {
      if(attackOnKing(moves.moves[i], boards)) return false;
    }
    return true;
  }

  moveList generateLegalMoves() {
    moveList moves = generateSemilegalMoves();
    moveList returnedMoves;
    for(int i = 0; i < moves.length; i++) {
      makeMove(moves.moves[i]);
      if(isGameLegal()) returnedMoves.addMove(moves.moves[i]);
      undoMove();
    }
    return returnedMoves;
  }

  moveList generateSemilegalMoves() {
    if(blacksTurn) {
      return possibleMovesB(history, boards);
    } else {
      return possibleMovesW(history, boards);
    }
  }
};
