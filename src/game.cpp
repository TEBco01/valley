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

#include <game.h>

static inline U64 positionMask(byte square) { // Func borrowed from moveGeneration
  return 1ULL << (63ULL - (U64)square);
}

static bool pieceAtSquare(const U64 board, const int square) { // Func borrowed from moveGeneration
  if( board & positionMask(square) ) {
    return true;
  } else {
    return false;
  }
}

void game::generateHash() {
  U64 state = 0ULL;

  for(int i = 0; i < 64; i++)
  {
    if(pieceAtSquare(boards.WP, i))
      state ^= tt->hashWP[i];
  }
  for(int i = 0; i < 64; i++)
  {
    if(pieceAtSquare(boards.WN, i))
      state ^= tt->hashWN[i];
  }
  for(int i = 0; i < 64; i++)
  {
    if(pieceAtSquare(boards.WB, i))
      state ^= tt->hashWB[i];
  }
  for(int i = 0; i < 64; i++)
  {
    if(pieceAtSquare(boards.WR, i))
      state ^= tt->hashWR[i];
  }
  for(int i = 0; i < 64; i++)
  {
    if(pieceAtSquare(boards.WQ, i))
      state ^= tt->hashWQ[i];
  }
  for(int i = 0; i < 64; i++)
  {
    if(pieceAtSquare(boards.WK, i))
      state ^= tt->hashWK[i];
  }
  for(int i = 0; i < 64; i++)
  {
    if(pieceAtSquare(boards.BP, i))
      state ^= tt->hashBP[i];
  }
  for(int i = 0; i < 64; i++)
  {
    if(pieceAtSquare(boards.BN, i))
      state ^= tt->hashBN[i];
  }
  for(int i = 0; i < 64; i++)
  {
    if(pieceAtSquare(boards.BB, i))
      state ^= tt->hashBB[i];
  }
  for(int i = 0; i < 64; i++)
  {
    if(pieceAtSquare(boards.BR, i))
      state ^= tt->hashBR[i];
  }
  for(int i = 0; i < 64; i++)
  {
    if(pieceAtSquare(boards.BQ, i))
      state ^= tt->hashBQ[i];
  }
  for(int i = 0; i < 64; i++)
  {
    if(pieceAtSquare(boards.BK, i))
      state ^= tt->hashBK[i];
  }
  if(blacksTurn)
    state ^= tt->hashSide;
  if(boards.castleInfo.whiteACan)
    state ^= tt->hashWhiteA;
  if(boards.castleInfo.whiteHCan)
    state ^= tt->hashWhiteH;
  if(boards.castleInfo.blackACan)
    state ^= tt->hashBlackA;
  if(boards.castleInfo.blackHCan)
    state ^= tt->hashBlackH;

  move lastMove = history.peek();
  int file = -1;
  if(!(lastMove.start == 0 && lastMove.end == 0)) {
    if(24 <= lastMove.end && lastMove.end <= 31) {
      if(pieceAtSquare(boards.BP, lastMove.end) && 8 <= lastMove.start && lastMove.start <= 15) {
        file = lastMove.end % 8;
      }
    }
  }
  if(!(lastMove.start == 0 && lastMove.end == 0)) {
		if(32 <= lastMove.end && lastMove.end <= 39) {
			if(pieceAtSquare(boards.WP, lastMove.end)  && 48 <= lastMove.start && lastMove.start <= 55) {
        file = lastMove.end % 8;
      }
    }
  }
  if(file != -1)
    state ^= tt->hashEnPassant[file];

  hash = state;
}

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
