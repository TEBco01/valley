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
#include <iostream> // For debugging only

/* template for iterating through linkedMoveList

moveNode* i = moves.head;
while(i != NULL) {
  // loop code
  i = i->next;
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

void moveStack::push(move a){
	movePancake *pancake = new movePancake;
	pancake->data = a;
	pancake->last = tail;
	tail = pancake;
};

move moveStack::pop(){
	movePancake *newtail = tail->last;
	move moves = tail->data;
	delete tail;
	tail = newtail;
	return moves;
};

void moveStack::emptyPop(){
	movePancake *newtail = tail->last;
	delete tail;
	tail = newtail;
};

move moveStack::peek(){
  if(tail != NULL)
	 return tail->data;
  else
  {
    move emptyMove;
    return emptyMove;
  }
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

  bool isGameLegal() {
    linkedMoveList moves = generateSemilegalMoves(); // TODO: Big optimization by reusing these generated moves if the depth increases

    moveNode* i = moves.head;
    while(i != NULL) {
      if(attackOnKing(i->data, boards))
      {
        moves.deleteList();
        return false;
      }
      i = i->next;
    }
    moves.deleteList();
    return true;
  }

  linkedMoveList generateLegalMoves() {
    linkedMoveList moves = generateSemilegalMoves();
    linkedMoveList returnedMoves;

    moveNode* i = moves.head;
    while(i != NULL) {
      makeMove(i->data);
      if(isGameLegal()) returnedMoves.add(i->data);
      undoMove();
      i = i->next;
    }
    moves.deleteList();
    return returnedMoves;
  }

  linkedMoveList generateSemilegalMoves() {
    if(blacksTurn) {
      return possibleMovesB(history.peek(), boards);
    } else {
      return possibleMovesW(history.peek(), boards);
    }
  }
};
