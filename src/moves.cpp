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

#include <moves.h>
#include <iostream>

void linkedMoveList::add(move value){
	moveNode *temp = new moveNode;
	temp->data = value;
	temp->next = NULL;
	if(head == NULL)
	{
		head = temp;
		tail = temp;
		temp = NULL;
	}
	else
	{
		tail->next = temp;
		tail = temp;
	}
};

void linkedMoveList::create(byte start, byte end, byte special){
	move newMove;
	newMove.start = start;
	newMove.end = end;
	newMove.special = special;
	this->add(newMove);
};

void linkedMoveList::remove(int pos){
	if(pos == 0) {
		moveNode *temp = head;
		head = head->next;
		delete temp;
	} else {
		moveNode *current = head->next;
		moveNode *previous = head;
		for(int i = 1; i < pos; i++){
			previous = current;
			current = current->next;
		}
		previous->next = current->next;
		if(current == tail) tail = previous;
		delete current;
	}
};

void linkedMoveList::operator=(linkedMoveList b){
	this->head = b.head;
	this->tail = b.tail;
};

void linkedMoveList::deleteList(){
  moveNode *cur;
  moveNode *temp;
  cur = head;
	while(temp != NULL){
    temp = cur->next;
		delete cur;
    cur = temp;
	}

  head = NULL;
  tail = NULL;
};

void linkedMoveList::operator+=(linkedMoveList b){
  if(this->head == NULL) {
    *this = b;
  } else {
    this->tail->next = b.head;
    if(b.head != NULL)
      this->tail = b.tail;
  }
};

linkedMoveList generateMovesFromBitboard(const U64 toBoard) {
  linkedMoveList moves;
  for (int i=0; i < 64; i++) {
    if( toBoard & 1ULL << (63ULL - (U64)i) ) {
      moves.create(0, i);
    }
  }
  return moves;
}

void constantShiftGenerator(U64 board, int shift, linkedMoveList& possibleMoves) {
  linkedMoveList moves = generateMovesFromBitboard(board);
  moveNode* i = moves.head;
  while(i != NULL) {
    i->data.start = i->data.end + shift;
    i = i->next;
  }
  possibleMoves += moves;
}

void constantShiftGenerator(U64 board, int shift, int special, linkedMoveList& possibleMoves) {
  linkedMoveList moves = generateMovesFromBitboard(board);
  moveNode* i = moves.head;
  while(i != NULL) {
    i->data.start = i->data.end + shift;
    i->data.special = special;
    i = i->next;
  }
  possibleMoves += moves;
}

linkedMoveList possibleMovesWPawns(move lastMove, const bitboards game, const extraBitboardsInfo info) {
  linkedMoveList possibleMoves;
  // Move forward one
  U64 fw1 = ( (game.WP & ~Rank_7) << 8 ) & ~(info.BlackPieces | info.WhitePieces);
  constantShiftGenerator(fw1, 8, possibleMoves);

  // Move forward two
  U64 fw2 = ((game.WP & Rank_2) << 16) & ~(info.BlackPieces | info.WhitePieces) & ~((info.BlackPieces | info.WhitePieces) << 8);
  constantShiftGenerator(fw2, 16, possibleMoves);

  // Attack
  U64 atL = ((game.WP & ~File_A) << 9) & info.BlackPieces;
  constantShiftGenerator(atL, 9, possibleMoves);

  U64 atR = ((game.WP & ~File_H) << 7) & info.BlackPieces;
  constantShiftGenerator(atR, 7, possibleMoves);

  // Promotion
  U64 pr = ((game.WP & Rank_7) << 8) & ~(info.BlackPieces | info.WhitePieces);
  linkedMoveList prList = generateMovesFromBitboard(pr);
  linkedMoveList prExtraList;
  moveNode* i = prList.head;
  while(i != NULL) {
    i->data.start = i->data.end + 8;
    i->data.special = 2;
    for(byte j = 3; j <= 5; j++) {
      prExtraList.create(i->data.start, i->data.end, j);
    }
    i = i->next;
  }
  possibleMoves += prList;
  possibleMoves += prExtraList;

  // En passant

  /*U64 ep = (game.BP & Rank_5) << 8;

  U64 epL = ep & ((game.WP & ~File_A) << 9);
  linkedMoveList epLList = generateMovesFromBitboard(epL);

  i = epLList.head;
  moveNode* previous = NULL;
  while(i != NULL) {
    if(!(history.tail->data.end == i->data.end + 8)) { // Checks to see if enemy pawn moved last
      previous->next = i->next;
      delete i;
      i = previous;
    }
    else {
      i->data.start = i->data.end + 9;
      i->data.special = 10;
    }
    previous = i;
    i = i->next;
  }
  possibleMoves += epLList;

  U64 epR = ep & ((game.WP & ~File_H) << 7);
  linkedMoveList epRList = generateMovesFromBitboard(epR);
  i = epRList.head;
  previous = NULL;
  while(i != NULL) {
    if(!(history.tail->data.end == i->data.end + 8)) {
      previous->next = i->next;
      delete i;
      i = previous;
    }
    else {
      i->data.start = i->data.end + 9;
      i->data.special = 10;
    }
    previous = i;
    i = i->next;
  }
  possibleMoves += epRList;*/

  return possibleMoves;
}

linkedMoveList possibleMovesBPawns(move lastMove, const bitboards game, const extraBitboardsInfo info) {
  linkedMoveList possibleMoves;
  // Move forward one
  U64 fw1 = ( (game.BP & ~Rank_2) >> 8 ) & ~(info.BlackPieces | info.WhitePieces);
  constantShiftGenerator(fw1, -8, possibleMoves);

  // Move forward two
  U64 fw2 = ((game.BP & Rank_7) >> 16) & ~(info.BlackPieces | info.WhitePieces) & ~((info.BlackPieces | info.WhitePieces) >> 8);
  constantShiftGenerator(fw2, -16, possibleMoves);

  // Attack
  U64 atL = ((game.BP & ~File_H) >> 9) & info.WhitePieces;
  constantShiftGenerator(atL, -9, possibleMoves);

  U64 atR = ((game.BP & ~File_A) >> 7) & info.WhitePieces;
  constantShiftGenerator(atR, -7, possibleMoves);

  // Promotion
  U64 pr = ((game.BP & Rank_2) >> 8) & ~(info.BlackPieces | info.WhitePieces);
  linkedMoveList prList = generateMovesFromBitboard(pr);
  linkedMoveList prExtraList;
  moveNode* i = prList.head;
  while(i != NULL) {
    i->data.start = i->data.end - 8;
    i->data.special = 2;
    for(byte j = 3; j <= 5; j++) {
      prExtraList.create(i->data.start, i->data.end, j);
    }
    i = i->next;
  }
  possibleMoves += prList;
  possibleMoves += prExtraList;

  // En passant
  U64 ep = (game.BP & Rank_4) >> 8;

  U64 epL = ep & ((game.BP & ~File_H) >> 9);
  linkedMoveList epLList = generateMovesFromBitboard(epL);

  i = epLList.head;
  moveNode* previous = NULL;
  while(i != NULL) {
    if(!(history.tail->data.end == i->data.end - 8)) { // Checks to see if enemy pawn moved last
      previous->next = i->next;
      delete i;
      i = previous;
    }
    else {
      i->data.start = i->data.end - 9;
      i->data.special = 10;
    }
    previous = i;
    i = i->next;
  }
  possibleMoves += epLList;

  U64 epR = ep & ((game.BP & ~File_A) << 7);
  linkedMoveList epRList = generateMovesFromBitboard(epR);
  i = epRList.head;
  previous = NULL;
  while(i != NULL) {
    if(!(history.tail->data.end == i->data.end - 8)) {
      previous->next = i->next;
      delete i;
      i = previous;
    }
    else {
      i->data.start = i->data.end -7;
      i->data.special = 10;
    }
    previous = i;
    i = i->next;
  }
  possibleMoves += epRList;

  return possibleMoves;
}

linkedMoveList possibleMovesKnights(const U64 kBoard, const U64 FriendlyPieces) {
  linkedMoveList possibleMoves;

  //Left two up one
  U64 l2u1 = ((kBoard & ~(File_A | File_B | Rank_8)) << 10) & ~FriendlyPieces; // Posible trival optimization by precomupting const bitboard combinations
  constantShiftGenerator(l2u1, 10, possibleMoves);

  //Right two up one
  U64 r2u1 = ((kBoard & ~(File_G | File_H | Rank_8)) << 6) & ~FriendlyPieces;
  constantShiftGenerator(r2u1, 6, possibleMoves);

  //Left one up two
  U64 l1u2 = ((kBoard & ~(File_A | Rank_7 | Rank_8)) << 17) & ~FriendlyPieces;
  constantShiftGenerator(l1u2, 17, possibleMoves);

  //Right one up two
  U64 r1u2 = ((kBoard & ~(File_H | Rank_7 | Rank_8)) << 15) & ~FriendlyPieces;
  constantShiftGenerator(r1u2, 15, possibleMoves);

  //Left two down one
  U64 l2d1 = ((kBoard & ~(File_A | File_B | Rank_1)) >> 6) & ~FriendlyPieces;
  constantShiftGenerator(l2d1, -6, possibleMoves);

  //Right two down one
  U64 r2d1 = ((kBoard & ~(File_G | File_H | Rank_1)) >> 10) & ~FriendlyPieces;
  constantShiftGenerator(r2d1, -10, possibleMoves);

  //Left one down two
  U64 l1d2 = ((kBoard & ~(File_A | Rank_1 | Rank_2)) >> 15) & ~FriendlyPieces;
  constantShiftGenerator(l1d2, -15, possibleMoves);

  //Right one down two
  U64 r1d2 = ((kBoard & ~(File_H | Rank_1 | Rank_2)) >> 17) & ~FriendlyPieces;
  constantShiftGenerator(r1d2, -17, possibleMoves);

  return possibleMoves;
}

linkedMoveList possibleMovesWKnights(const bitboards game, const extraBitboardsInfo info) {
  return possibleMovesKnights(game.WN, info.WhitePieces);
}

linkedMoveList possibleMovesBKnights(const bitboards game, const extraBitboardsInfo info) {
  return possibleMovesKnights(game.BN, info.BlackPieces);
}

linkedMoveList possibleMovesKings(const U64 kBoard, const U64 FriendlyPieces) {
  linkedMoveList possibleMoves;

  U64 u = ((kBoard & ~(File_H)) << 8) & ~FriendlyPieces;
  constantShiftGenerator(u, 8, possibleMoves);

  U64 ur = ((kBoard & ~(File_H | Rank_8)) << 7) & ~FriendlyPieces;
  constantShiftGenerator(ur, 7, possibleMoves);

  U64 ul = ((kBoard & ~(File_H | Rank_1)) << 9) & ~FriendlyPieces;
  constantShiftGenerator(ul, 9, possibleMoves);

  U64 l = ((kBoard & ~(Rank_1)) << 1) & ~FriendlyPieces;
  constantShiftGenerator(l, 1, possibleMoves);

  U64 r = ((kBoard & ~(File_H | Rank_1)) >> 1) & ~FriendlyPieces;
  constantShiftGenerator(r, -1, possibleMoves);

  U64 d = ((kBoard & ~(File_A)) >> 8) & ~FriendlyPieces;
  constantShiftGenerator(d, -8, possibleMoves);

  U64 dr = ((kBoard & ~(File_A | Rank_8)) >> 9) & ~FriendlyPieces;
  constantShiftGenerator(dr, -9, possibleMoves);

  U64 dl = ((kBoard & ~(File_H | Rank_1)) >> 7) & ~FriendlyPieces;
  constantShiftGenerator(dl, -7, possibleMoves);

  return possibleMoves;
}

linkedMoveList possibleMovesWKings(const bitboards game, const extraBitboardsInfo info) {
  return possibleMovesKings(game.WK, info.WhitePieces);
}

linkedMoveList possibleMovesBKings(const bitboards game, const extraBitboardsInfo info) {
  return possibleMovesKings(game.BK, info.BlackPieces);
}

bool pieceAtSquare(const U64 board, const int square) {
  if( board & 1ULL << (63ULL - (U64)square) ) {
    return true;
  } else {
    return false;
  }
}

// TODO: Optimization possible. Reduction from ints? Mask consolidation?
linkedMoveList rayAttack(const int origin, const int modifier, const U64 NoNoZone, const U64 enemies, const U64 friendlies) {
  linkedMoveList possibleMoves;
  int cursor = origin;

  while (true) {
    if(pieceAtSquare(NoNoZone, cursor)) {
      break;
    }

    cursor += modifier;

    if(pieceAtSquare(friendlies, cursor)) {
      break;
    }
    if(pieceAtSquare(enemies, cursor)) {
      possibleMoves.create(origin, cursor);
      break;
    }
    possibleMoves.create(origin, cursor);
  }

  return possibleMoves;
}

linkedMoveList possibleMovesRooks(const U64 rBoard, const U64 FriendlyPieces, const U64 EnemyPieces) {
  linkedMoveList possibleMoves;
  linkedMoveList positions = generateMovesFromBitboard(rBoard);

  moveNode* i = positions.head;
  while(i != NULL) {
    possibleMoves += rayAttack((int)i->data.end, -8, Rank_8, EnemyPieces, FriendlyPieces);
    possibleMoves += rayAttack((int)i->data.end, 8, Rank_1, EnemyPieces, FriendlyPieces);
    possibleMoves += rayAttack((int)i->data.end, -1, File_A, EnemyPieces, FriendlyPieces);
    possibleMoves += rayAttack((int)i->data.end, 1, File_H, EnemyPieces, FriendlyPieces);
    i = i->next;
  }

  positions.deleteList();
  return possibleMoves;
}

linkedMoveList possibleMovesWRooks(const bitboards game, const extraBitboardsInfo info) {
  return possibleMovesRooks(game.WR, info.WhitePieces, info.BlackPieces);
}

linkedMoveList possibleMovesBRooks(const bitboards game, const extraBitboardsInfo info) {
  return possibleMovesRooks(game.BR, info.BlackPieces, info.WhitePieces);
}

linkedMoveList possibleMovesBishops(const U64 rBoard, const U64 FriendlyPieces, const U64 EnemyPieces) {
  linkedMoveList possibleMoves;
  linkedMoveList positions = generateMovesFromBitboard(rBoard);

  moveNode* i = positions.head;
  while(i != NULL) {
    possibleMoves += rayAttack((int)i->data.end, -9, Rank_8 | File_A, EnemyPieces, FriendlyPieces);
    possibleMoves += rayAttack((int)i->data.end, -7, Rank_8 | File_H, EnemyPieces, FriendlyPieces);
    possibleMoves += rayAttack((int)i->data.end, 7, Rank_1 | File_A, EnemyPieces, FriendlyPieces);
    possibleMoves += rayAttack((int)i->data.end, 9, Rank_1 | File_H, EnemyPieces, FriendlyPieces);
    i = i->next;
  }

  positions.deleteList();
  return possibleMoves;
}

linkedMoveList possibleMovesWBishops(const bitboards game, const extraBitboardsInfo info) {
  return possibleMovesBishops(game.WB, info.WhitePieces, info.BlackPieces);
}

linkedMoveList possibleMovesBBishops(const bitboards game, const extraBitboardsInfo info) {
  return possibleMovesBishops(game.BB, info.BlackPieces, info.WhitePieces);
}

linkedMoveList possibleMovesQueens(const U64 qBoard, const U64 FriendlyPieces, const U64 EnemyPieces) {
  linkedMoveList possibleMoves;
  linkedMoveList positions = generateMovesFromBitboard(qBoard);

  moveNode* i = positions.head;
  while(i != NULL) {
    possibleMoves += rayAttack((int)i->data.end, -8, Rank_8, EnemyPieces, FriendlyPieces);
    possibleMoves += rayAttack((int)i->data.end, 8, Rank_1, EnemyPieces, FriendlyPieces);
    possibleMoves += rayAttack((int)i->data.end, -1, File_A, EnemyPieces, FriendlyPieces);
    possibleMoves += rayAttack((int)i->data.end, 1, File_H, EnemyPieces, FriendlyPieces);
    possibleMoves += rayAttack((int)i->data.end, -9, Rank_8 | File_A, EnemyPieces, FriendlyPieces);
    possibleMoves += rayAttack((int)i->data.end, -7, Rank_8 | File_H, EnemyPieces, FriendlyPieces);
    possibleMoves += rayAttack((int)i->data.end, 7, Rank_1 | File_A, EnemyPieces, FriendlyPieces);
    possibleMoves += rayAttack((int)i->data.end, 9, Rank_1 | File_H, EnemyPieces, FriendlyPieces);
    i = i->next;
  }

  positions.deleteList();
  return possibleMoves;
}

linkedMoveList possibleMovesWQueens(const bitboards game, const extraBitboardsInfo info) {
  return possibleMovesQueens(game.WQ, info.WhitePieces, info.BlackPieces);
}

linkedMoveList possibleMovesBQueens(const bitboards game, const extraBitboardsInfo info) {
  return possibleMovesQueens(game.BQ, info.BlackPieces, info.WhitePieces);
}

// Only guaranteed for standard chess
linkedMoveList possibleMovesW(move lastMove, const bitboards game) {
  linkedMoveList possibleMoves;

  extraBitboardsInfo info;
  info.updateFromBitboards(game);

  possibleMoves += possibleMovesWPawns(lastMove, game, info);
  possibleMoves += possibleMovesWKnights(game, info);
  possibleMoves += possibleMovesWKings(game, info);
  possibleMoves += possibleMovesWRooks(game, info);
  possibleMoves += possibleMovesWBishops(game, info);
  possibleMoves += possibleMovesWQueens(game, info);

  return possibleMoves;
}

linkedMoveList possibleMovesB(move lastMove, const bitboards game) {
  linkedMoveList possibleMoves;

  extraBitboardsInfo info;
  info.updateFromBitboards(game);

  possibleMoves += possibleMovesBPawns(lastMove, game, info);
  possibleMoves += possibleMovesBKnights(game, info);
  possibleMoves += possibleMovesBKings(game, info);
  possibleMoves += possibleMovesBRooks(game, info);
  possibleMoves += possibleMovesBBishops(game, info);
  possibleMoves += possibleMovesBQueens(game, info);

  return possibleMoves;
}

void setAllToZero(bitboards* game, U64 endMask) {
  U64 mask = ~endMask;
  game->WP &= mask;
  game->WN &= mask;
  game->WB &= mask;
  game->WR &= mask;
  game->WQ &= mask;
  game->WK &= mask;
  game->BP &= mask;
  game->BN &= mask;
  game->BB &= mask;
  game->BR &= mask;
  game->BQ &= mask;
  game->BK &= mask;
}

void checkBoardMove(U64& board, U64 startMask, U64 endMask) {
  if( board & startMask ) {
    board &= ~startMask;
    board |= endMask;
  }
}

// Note: lots of room for optimization
void applyMove(bitboards* game, move change) {
  U64 startMask = 1ULL << (63ULL - (U64)change.start);
  U64 endMask = 1ULL << (63ULL - (U64)change.end);

  setAllToZero(game, endMask);

  checkBoardMove(game->WP, startMask, endMask);
  checkBoardMove(game->WN, startMask, endMask);
  checkBoardMove(game->WB, startMask, endMask);
  checkBoardMove(game->WR, startMask, endMask);
  checkBoardMove(game->WQ, startMask, endMask);
  checkBoardMove(game->WK, startMask, endMask);
  checkBoardMove(game->BP, startMask, endMask);
  checkBoardMove(game->BN, startMask, endMask);
  checkBoardMove(game->BB, startMask, endMask);
  checkBoardMove(game->BR, startMask, endMask);
  checkBoardMove(game->BQ, startMask, endMask);
  checkBoardMove(game->BK, startMask, endMask);

  switch(change.special) {
    case 2:
      setAllToZero(game, endMask);
      game->WQ |= endMask;
      break;
    case 3:
      setAllToZero(game, endMask);
      game->WR |= endMask;
      break;
    case 4:
      setAllToZero(game, endMask);
      game->WN |= endMask;
      break;
    case 5:
      setAllToZero(game, endMask);
      game->WB |= endMask;
      break;
    case 6:
      setAllToZero(game, endMask);
      game->BQ |= endMask;
      break;
    case 7:
      setAllToZero(game, endMask);
      game->BR |= endMask;
      break;
    case 8:
      setAllToZero(game, endMask);
      game->BN |= endMask;
      break;
    case 9:
      setAllToZero(game, endMask);
      game->BB |= endMask;
      break;
    case 10:
      setAllToZero(game, endMask >> 8);
      break;
    case 11:
      setAllToZero(game, endMask << 8);
      break;
    default:
      break;
  }
}

bool attackOnKing(move change, const bitboards game) {
  U64 endMask = 1ULL << (63ULL - (U64)change.end);

  if(game.WK & endMask || game.BK & endMask) {
    return true;
  }
  return false;
}
