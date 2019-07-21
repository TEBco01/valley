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

#include <bitboard.h>
#include <moves.h>
#include <moveGeneration.h>

/* template for iterating through linkedMoveList

moveNode* i = moves.head;
while(i != NULL) {
  // loop code
  i = i->next;
}

*/

/* template for iterating through arrayMoveList

list.resetIterator();
while(list.next()) {
  // loop code
  // Use the getMove and remove functions to interact
}

*/

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

struct movePancake {
	move data;
	movePancake *last;
};

class moveStack {
	public:
		void push(move a);
		move pop(); // TODO: Would an empty pop be more efficient? We never need to peek while we pop
   	void emptyPop();
    move peek();
	private:
		movePancake *tail = NULL;
};

struct game {
  bitboards boards;
  boardStack boardHistory;

  moveStack history;

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
    history.push(moveMade);
    blacksTurn ^= 1;
  }
  void undoMove() {
    boards = boardHistory.pop();
    history.emptyPop();
    blacksTurn ^= 1;
  }

  bool isGameLegal() { // Most of this function can be moved to or shared with the "inCheck" function
    arrayMoveList moves = generateSemilegalMoves(); // TODO: Big optimization by reusing these generated moves if the depth increases

    moves.resetIterator();
    while(moves.next()) {
      if(attackOnKing(moves.getMove(), boards))
      {
        return false;
      }
    }
    return true;
  }

  arrayMoveList generateLegalMoves() {
    arrayMoveList moves = generateSemilegalMoves();
    arrayMoveList returnedMoves;

    moves.resetIterator();
    while(moves.next()) {
      if(moves.getMove().special == 1) { // Are castling kings put in check en route?
        bool good = true;
        makeMove(moves.getMove());
        if(!isGameLegal()) good = false;
        undoMove();

        if(inCheck(blacksTurn, boards)) good = false;

        move intermediate;
        switch (moves.getMove().end) {
          case 2:
            intermediate.start = moves.getMove().start;
            intermediate.end = 3;
            makeMove(intermediate);
              if(!isGameLegal()) good = false;
            undoMove();
          break;
          case 6:
            intermediate.start = moves.getMove().start;
            intermediate.end = 5;
            makeMove(intermediate);
              if(!isGameLegal()) good = false;
            undoMove();
          break;
          case 58:
            intermediate.start = moves.getMove().start;
            intermediate.end = 59;
            makeMove(intermediate);
              if(!isGameLegal()) good = false;
            undoMove();
          break;
          case 62:
            intermediate.start = moves.getMove().start;
            intermediate.end = 61;
            makeMove(intermediate);
              if(!isGameLegal()) good = false;
            undoMove();
          break;
        }
        if(good) returnedMoves.add(moves.getMove());
      } else {
        makeMove(moves.getMove());
        if(isGameLegal()) returnedMoves.add(moves.getMove());
        undoMove();
      }
    }
    return returnedMoves;
  }

  arrayMoveList generateSemilegalMoves() {
    if(blacksTurn) {
      return possibleMovesB(history.peek(), boards);
    } else {
      return possibleMovesW(history.peek(), boards);
    }
  }
};
