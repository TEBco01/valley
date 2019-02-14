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

#include <moves.h>
#include <iostream>
//#include <bitboard.h> // For testing only

struct infoBoards {
  /*C64 FILE_A = 16843009ULL;
  C64 FILE_H = 33686018ULL;
  C64 FILE_AB = 18446744071587332481ULL;
  C64 FILE_GH = 101058054ULL;
  C64 RANK_1 = 510ULL;
  C64 RANK_4 = 18446744073675997185ULL;
  C64 RANK_5 = 0;
  C64 RANK_8 = 0;
  C64 CENTER = 0;
  C64 EXTENDED_CENTER = 0;
  C64 KING_SIDE = 0;
  C64 QUEEN_SIDE = 0;
  C64 KING_B7 = 0;
  C64 KNIGHT_C6 = 0;
  U64 NOT_WHITE_PIECES;
  U64 BLACK_PIECES;
  U64 EMPTY;*/
};

bool isLowercase(char character) {
  if(character >= 97 && character <= 122) {
    return true;
  } else {
    return false;
  }
}

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

void linkedMoveList::create(byte start, byte end, byte special = 0){
	move newMove;
	newMove.start = start;
	newMove.end = end;
	newMove.special = special;
	this->add(newMove);
};

void linkedMoveList::remove(int pos){
	moveNode *current = new moveNode;
	moveNode *previous = new moveNode;
	current = head;
	for(int i = 1; i < pos; i++){
		previous = current;
		current = current->next;
	}
	previous->next = current->next;
	delete current;
};

void linkedMoveList::operator=(linkedMoveList b){
	this->head = b.head;
	this->tail = b.tail;
};

/*linkedMoveList::~linkedMoveList(){
  moveNode *cur;
  moveNode *temp;
  cur = head;
	while(temp != NULL){
    temp = cur->next;
		delete cur;
    cur = temp;
	}
};*/

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

linkedMoveList linkedMoveList::operator+(linkedMoveList b){
	linkedMoveList a;
	a.head = this->head;
	a.tail = this->tail;
	a.tail->next = b.head;
	a.tail = b.tail;
	return a;
};

void linkedMoveList::operator+=(linkedMoveList b){
	 *this = *this + b;
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

void constantShiftGenerator(U64 board, int shift, &moveList possibleMoves) {
  linkedMoveList moves = generateMovesFromBitboard(board);
  moveNode* i = moves.head;
  while(i != NULL) {
    i->data.start = i->data.end + shift;
    i = i->next;
  }
  possibleMoves += moves;
}

moveList possibleMovesWPawns(moveList history, const bitboards game, const extraBitboardsInfo info) {
  moveList possibleMoves;
  // Move forward one
  U64 fw1 = ( (game.WP & ~Rank_7) << 8 ) & ~(info.BlackPieces | info.WhitePieces);
  moveList fw1List = generateMovesFromBitboard(fw1);
  for(int i = 0; i < fw1List.length; i++) {
    fw1List.moves[i].start = fw1List.moves[i].end + 8;
  }
  possibleMoves.addMoveList(fw1List);

  // Move forward two
  U64 fw2 = ((game.WP & Rank_2) << 16) & ~(info.BlackPieces | info.WhitePieces) & ~((info.BlackPieces | info.WhitePieces) << 8);
  moveList fw2List = generateMovesFromBitboard(fw2);
  for(int i = 0; i < fw2List.length; i++) {
    fw2List.moves[i].start = fw2List.moves[i].end + 16;
  }
  possibleMoves.addMoveList(fw2List);

  // Attack
  U64 atL = ((game.WP & ~File_A) << 9) & info.BlackPieces;
  moveList atLList = generateMovesFromBitboard(atL);
  for(int i = 0; i < atLList.length; i++) {
    atLList.moves[i].start = atLList.moves[i].end + 9;
  }
  possibleMoves.addMoveList(atLList);

  U64 atR = ((game.WP & ~File_H) << 7) & info.BlackPieces;
  moveList atRList = generateMovesFromBitboard(atR);
  for(int i = 0; i < atRList.length; i++) {
    atRList.moves[i].start = atRList.moves[i].end + 7;
  }
  possibleMoves.addMoveList(atRList);

  // Promotion
  U64 pr = ((game.WP & Rank_7) << 8) & ~(info.BlackPieces | info.WhitePieces);
  moveList prList = generateMovesFromBitboard(pr);
  int originalLength = prList.length;
  for(int i = 0; i < originalLength; i++) {
    byte end = prList.moves[i].end;
    byte start = end + 8;
    prList.moves[i].start = start;
    prList.moves[i].special = 2;
    for(byte j = 3; j <= 5; j++) {
      prList.createMove(start, end, j);
    }
  }
  possibleMoves.addMoveList(prList);

  // En passant
  U64 ep = (game.BP & Rank_5) << 8;

  U64 epL = ep & ((game.WP & ~File_A) << 9);
  moveList epLList = generateMovesFromBitboard(epL);
  for(int i = 0; i < epLList.length; i++) {
    epLList.moves[i].start = epLList.moves[i].end + 9;
    epLList.moves[i].special = 10;
    if(!(history.moves[history.length-1].end == epLList.moves[i].end + 8)) { // Checks to see if the enemy pawn moved last
      epLList.removeMove(i);
      i--;
    }
  }
  possibleMoves.addMoveList(epLList);

  U64 epR = ep & ((game.WP & ~File_H) << 7);
  moveList epRList = generateMovesFromBitboard(epR);
  for(int i = 0; i < epRList.length; i++) {
    epRList.moves[i].start = epRList.moves[i].end + 7;
    epRList.moves[i].special = 10;
    if(!(history.moves[history.length-1].end == epRList.moves[i].end + 8)) { // Checks to see if the enemy pawn moved last
      epRList.removeMove(i);
      i--;
    }
  }
  possibleMoves.addMoveList(epRList);

  return possibleMoves;
}

moveList possibleMovesBPawns(moveList history, const bitboards game, const extraBitboardsInfo info) {
  moveList possibleMoves;
  // Move forward one
  U64 fw1 = ( (game.BP & ~Rank_2) >> 8 ) & ~(info.BlackPieces | info.WhitePieces);
  moveList fw1List = generateMovesFromBitboard(fw1);
  for(int i = 0; i < fw1List.length; i++) {
    fw1List.moves[i].start = fw1List.moves[i].end - 8;
  }
  possibleMoves.addMoveList(fw1List);

  // Move forward two
  U64 fw2 = ((game.BP & Rank_7) >> 16) & ~(info.BlackPieces | info.WhitePieces) & ~((info.BlackPieces | info.WhitePieces) >> 8);
  moveList fw2List = generateMovesFromBitboard(fw2);
  for(int i = 0; i < fw2List.length; i++) {
    fw2List.moves[i].start = fw2List.moves[i].end - 16;
  }
  possibleMoves.addMoveList(fw2List);

  // Attack
  U64 atL = ((game.BP & ~File_H) >> 9) & info.WhitePieces;
  moveList atLList = generateMovesFromBitboard(atL);
  for(int i = 0; i < atLList.length; i++) {
    atLList.moves[i].start = atLList.moves[i].end - 9;
  }
  possibleMoves.addMoveList(atLList);

  U64 atR = ((game.BP & ~File_A) >> 7) & info.WhitePieces;
  moveList atRList = generateMovesFromBitboard(atR);
  for(int i = 0; i < atRList.length; i++) {
    atRList.moves[i].start = atRList.moves[i].end - 7;
  }
  possibleMoves.addMoveList(atRList);

  // Promotion
  U64 pr = ((game.BP & Rank_2) >> 8) & ~(info.BlackPieces | info.WhitePieces);
  moveList prList = generateMovesFromBitboard(pr);
  int originalLength = prList.length;
  for(int i = 0; i < originalLength; i++) {
    byte end = prList.moves[i].end;
    byte start = end - 8;
    prList.moves[i].start = start;
    prList.moves[i].special = 2;
    for(byte j = 3; j <= 5; j++) {
      prList.createMove(start, end, j);
    }
  }
  possibleMoves.addMoveList(prList);

  // En passant
  U64 ep = (game.BP & Rank_4) >> 8;

  U64 epL = ep & ((game.BP & ~File_H) >> 9);
  moveList epLList = generateMovesFromBitboard(epL);
  for(int i = 0; i < epLList.length; i++) {
    epLList.moves[i].start = epLList.moves[i].end - 9;
    epLList.moves[i].special = 10;
    if(!(history.moves[history.length-1].end == epLList.moves[i].end - 8)) { // Checks to see if the enemy pawn moved last
      epLList.removeMove(i);
      i--;
    }
  }
  possibleMoves.addMoveList(epLList);

  U64 epR = ep & ((game.BP & ~File_A) << 7);
  moveList epRList = generateMovesFromBitboard(epR);
  for(int i = 0; i < epRList.length; i++) {
    epRList.moves[i].start = epRList.moves[i].end - 7;
    epRList.moves[i].special = 10;
    if(!(history.moves[history.length-1].end == epRList.moves[i].end - 8)) { // Checks to see if the enemy pawn moved last
      epRList.removeMove(i);
      i--;
    }
  }
  possibleMoves.addMoveList(epRList);

  return possibleMoves;
}

linkedMoveList possibleMovesKnights(const U64 kBoard, const U64 FriendlyPieces) {
  linkedMoveList possibleMoves;

  //Left two up one
  U64 l2u1 = ((kBoard & ~(File_A | File_B | Rank_8)) << 10) & ~FriendlyPieces; // Posible trival optimization by precomupting const bitboard combinations
  linkedMoveList l2u1List = generateMovesFromBitboard(l2u1);
  moveNode* i = l2u1List.head;
  while(i != NULL) {
    i->data.start = i->data.end + 10;
    i = i->next;
  }
  possibleMoves += l2u1List;

  //Right two up one
  U64 r2u1 = ((kBoard & ~(File_G | File_H | Rank_8)) << 6) & ~FriendlyPieces;
  linkedMoveList r2u1List = generateMovesFromBitboard(r2u1);
  i = r2u1List.head;
  while(i != NULL) {
    i->data.start = i->data.end + 6;
    i = i->next;
  }
  possibleMoves += r2u1List;

  //Left one up two
  U64 l1u2 = ((kBoard & ~(File_A | Rank_7 | Rank_8)) << 17) & ~FriendlyPieces;
  linkedMoveList l1u2List = generateMovesFromBitboard(l1u2);
  i = l1u2List.head;
  while(i != NULL) {
    i->data.start = i->data.end + 17;
    i = i->next;
  }
  possibleMoves += l1u2List;

  //Right one up two
  U64 r1u2 = ((kBoard & ~(File_H | Rank_7 | Rank_8)) << 15) & ~FriendlyPieces;
  linkedMoveList r1u2List = generateMovesFromBitboard(r1u2);
  i = r1u2List.head;
  while(i != NULL) {
    i->data.start = i->data.end + 15;
    i = i->next;
  }
  possibleMoves += r1u2List;

  //Left two down one
  U64 l2d1 = ((kBoard & ~(File_A | File_B | Rank_1)) >> 6) & ~FriendlyPieces;
  linkedMoveList l2d1List = generateMovesFromBitboard(l2d1);
  i = l2d1List.head;
  while(i != NULL) {
    i->data.start = i->data.end - 6;
    i = i->next;
  }
  possibleMoves += l2d1List;

  //Right two down one
  U64 r2d1 = ((kBoard & ~(File_G | File_H | Rank_1)) >> 10) & ~FriendlyPieces;
  linkedMoveList r2d1List = generateMovesFromBitboard(r2d1);
  i = l2d1List.head;
  while(i != NULL) {
    i->data.start = i->data.end - 10;
    i = i->next;
  }
  possibleMoves += l2d1List;

  //Left one down two
  U64 l1d2 = ((kBoard & ~(File_A | Rank_1 | Rank_2)) >> 15) & ~FriendlyPieces;
  linkedMoveList l1d2List = generateMovesFromBitboard(l1d2);
  i = l1d2List.head;
  while(i != NULL) {
    i->data.start = i->data.end - 15;
    i = i->next;
  }
  possibleMoves += l1d2List;

  //Right one down two
  U64 r1d2 = ((kBoard & ~(File_H | Rank_1 | Rank_2)) >> 17) & ~FriendlyPieces;
  linkedMoveList r1d2List = generateMovesFromBitboard(r1d2);
  i = r1d2List.head;
  while(i != NULL) {
    i->data.start = i->data.end - 17;
    i = i->next;
  }
  possibleMoves += r1d2List;

  return possibleMoves;
}

moveList possibleMovesWKnights(const bitboards game, const extraBitboardsInfo info) {
  return possibleMovesKnights(game.WN, info.WhitePieces);
}

moveList possibleMovesBKnights(const bitboards game, const extraBitboardsInfo info) {
  return possibleMovesKnights(game.BN, info.BlackPieces);
}

moveList possibleMovesKings(const U64 kBoard, const U64 FriendlyPieces) {
  moveList possibleMoves;

  U64 u = ((kBoard & ~(File_H)) << 8) & ~FriendlyPieces;
  moveList uList = generateMovesFromBitboard(u);
  for(int i = 0; i < uList.length; i++) {
    uList.moves[i].start = uList.moves[i].end + 8;
  }
  possibleMoves.addMoveList(uList);

  U64 ur = ((kBoard & ~(File_H | Rank_8)) << 7) & ~FriendlyPieces;
  moveList urList = generateMovesFromBitboard(ur);
  for(int i = 0; i < urList.length; i++) {
    urList.moves[i].start = urList.moves[i].end + 7;
  }
  possibleMoves.addMoveList(urList);

  U64 ul = ((kBoard & ~(File_H | Rank_1)) << 9) & ~FriendlyPieces;
  moveList ulList = generateMovesFromBitboard(ul);
  for(int i = 0; i < ulList.length; i++) {
    ulList.moves[i].start = ulList.moves[i].end + 9;
  }
  possibleMoves.addMoveList(ulList);

  U64 l = ((kBoard & ~(Rank_1)) << 1) & ~FriendlyPieces;
  moveList lList = generateMovesFromBitboard(l);
  for(int i = 0; i < lList.length; i++) {
    lList.moves[i].start = lList.moves[i].end + 1;
  }
  possibleMoves.addMoveList(lList);

  U64 r = ((kBoard & ~(File_H | Rank_1)) >> 1) & ~FriendlyPieces;
  moveList rList = generateMovesFromBitboard(r);
  for(int i = 0; i < rList.length; i++) {
    rList.moves[i].start = rList.moves[i].end - 1;
  }
  possibleMoves.addMoveList(rList);

  U64 d = ((kBoard & ~(File_A)) >> 8) & ~FriendlyPieces;
  moveList dList = generateMovesFromBitboard(d);
  for(int i = 0; i < dList.length; i++) {
    dList.moves[i].start = dList.moves[i].end - 8;
  }
  possibleMoves.addMoveList(dList);

  U64 dr = ((kBoard & ~(File_A | Rank_8)) >> 9) & ~FriendlyPieces;
  moveList drList = generateMovesFromBitboard(dr);
  for(int i = 0; i < drList.length; i++) {
    drList.moves[i].start = drList.moves[i].end - 9;
  }
  possibleMoves.addMoveList(drList);

  U64 dl = ((kBoard & ~(File_H | Rank_1)) >> 7) & ~FriendlyPieces;
  moveList dlList = generateMovesFromBitboard(dl);
  for(int i = 0; i < dlList.length; i++) {
    dlList.moves[i].start = dlList.moves[i].end - 7;
  }
  possibleMoves.addMoveList(dlList);

  return possibleMoves;
}

moveList possibleMovesWKings(const bitboards game, const extraBitboardsInfo info) {
  return possibleMovesKings(game.WK, info.WhitePieces);
}

moveList possibleMovesBKings(const bitboards game, const extraBitboardsInfo info) {
  return possibleMovesKings(game.BK, info.BlackPieces);
}

bool pieceAtSquare(const U64 board, const int square) {
  if( board & 1ULL << (63ULL - (U64)square) ) {
    return true;
  } else {
    return false;
  }
}

moveList rayAttack(const int origin, const int modifier, const U64 NoNoZone, const U64 enemies, const U64 friendlies) {
  moveList possibleMoves;
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
      possibleMoves.createMove(origin, cursor);
      break;
    }
    possibleMoves.createMove(origin, cursor);
  }

  return possibleMoves;
}

moveList possibleMovesRooks(const U64 rBoard, const U64 FriendlyPieces, const U64 EnemyPieces) {
  moveList possibleMoves;
  moveList positions = generateMovesFromBitboard(rBoard);

  for(int i = 0; i < positions.length; i++) {
    possibleMoves.addMoveList(rayAttack((int)positions.moves[i].end, -8, Rank_8, EnemyPieces, FriendlyPieces));
    possibleMoves.addMoveList(rayAttack((int)positions.moves[i].end, 8, Rank_1, EnemyPieces, FriendlyPieces));
    possibleMoves.addMoveList(rayAttack((int)positions.moves[i].end, -1, File_A, EnemyPieces, FriendlyPieces));
    possibleMoves.addMoveList(rayAttack((int)positions.moves[i].end, 1, File_H, EnemyPieces, FriendlyPieces));
  }

  return possibleMoves;
}

moveList possibleMovesWRooks(const bitboards game, const extraBitboardsInfo info) {
  return possibleMovesRooks(game.WR, info.WhitePieces, info.BlackPieces);
}

moveList possibleMovesBRooks(const bitboards game, const extraBitboardsInfo info) {
  return possibleMovesRooks(game.BR, info.BlackPieces, info.WhitePieces);
}

moveList possibleMovesBishops(const U64 rBoard, const U64 FriendlyPieces, const U64 EnemyPieces) {
  moveList possibleMoves;
  moveList positions = generateMovesFromBitboard(rBoard);

  for(int i = 0; i < positions.length; i++) {
    possibleMoves.addMoveList(rayAttack((int)positions.moves[i].end, -9, Rank_8 | File_A, EnemyPieces, FriendlyPieces));
    possibleMoves.addMoveList(rayAttack((int)positions.moves[i].end, -7, Rank_8 | File_H, EnemyPieces, FriendlyPieces));
    possibleMoves.addMoveList(rayAttack((int)positions.moves[i].end, 7, Rank_1 | File_A, EnemyPieces, FriendlyPieces));
    possibleMoves.addMoveList(rayAttack((int)positions.moves[i].end, 9, Rank_1 | File_H, EnemyPieces, FriendlyPieces));
  }

  return possibleMoves;
}

moveList possibleMovesWBishops(const bitboards game, const extraBitboardsInfo info) {
  return possibleMovesBishops(game.WB, info.WhitePieces, info.BlackPieces);
}

moveList possibleMovesBBishops(const bitboards game, const extraBitboardsInfo info) {
  return possibleMovesBishops(game.BB, info.BlackPieces, info.WhitePieces);
}

moveList possibleMovesQueens(const U64 qBoard, const U64 FriendlyPieces, const U64 EnemyPieces) {
  moveList possibleMoves;
  moveList positions = generateMovesFromBitboard(qBoard);

  for(int i = 0; i < positions.length; i++) {
    possibleMoves.addMoveList(rayAttack((int)positions.moves[i].end, -8, Rank_8, EnemyPieces, FriendlyPieces));
    possibleMoves.addMoveList(rayAttack((int)positions.moves[i].end, 8, Rank_1, EnemyPieces, FriendlyPieces));
    possibleMoves.addMoveList(rayAttack((int)positions.moves[i].end, -1, File_A, EnemyPieces, FriendlyPieces));
    possibleMoves.addMoveList(rayAttack((int)positions.moves[i].end, 1, File_H, EnemyPieces, FriendlyPieces));
    possibleMoves.addMoveList(rayAttack((int)positions.moves[i].end, -9, Rank_8 | File_A, EnemyPieces, FriendlyPieces));
    possibleMoves.addMoveList(rayAttack((int)positions.moves[i].end, -7, Rank_8 | File_H, EnemyPieces, FriendlyPieces));
    possibleMoves.addMoveList(rayAttack((int)positions.moves[i].end, 7, Rank_1 | File_A, EnemyPieces, FriendlyPieces));
    possibleMoves.addMoveList(rayAttack((int)positions.moves[i].end, 9, Rank_1 | File_H, EnemyPieces, FriendlyPieces));
  }

  return possibleMoves;
}

moveList possibleMovesWQueens(const bitboards game, const extraBitboardsInfo info) {
  return possibleMovesQueens(game.WQ, info.WhitePieces, info.BlackPieces);
}

moveList possibleMovesBQueens(const bitboards game, const extraBitboardsInfo info) {
  return possibleMovesQueens(game.BQ, info.BlackPieces, info.WhitePieces);
}

// Only guaranteed for standard chess
moveList possibleMovesW(moveList history, const bitboards game) {
  moveList possibleMoves;

  extraBitboardsInfo info;
  info.updateFromBitboards(game);

  moveList pawnMoves = possibleMovesWPawns(history, game, info);
  possibleMoves.addMoveList(pawnMoves);

  moveList knightMoves = possibleMovesWKnights(game, info);
  possibleMoves.addMoveList(knightMoves);

  moveList kingMoves = possibleMovesWKings(game, info);
  possibleMoves.addMoveList(kingMoves);

  moveList rookMoves = possibleMovesWRooks(game, info);
  possibleMoves.addMoveList(rookMoves);

  moveList bishopMoves = possibleMovesWBishops(game, info);
  possibleMoves.addMoveList(bishopMoves);

  moveList queenMoves = possibleMovesWQueens(game, info);
  possibleMoves.addMoveList(queenMoves);

  return possibleMoves;
}

moveList possibleMovesB(moveList history, const bitboards game) {
  moveList possibleMoves;

  extraBitboardsInfo info;
  info.updateFromBitboards(game);

  moveList pawnMoves = possibleMovesBPawns(history, game, info);
  possibleMoves.addMoveList(pawnMoves);

  moveList knightMoves = possibleMovesBKnights(game, info);
  possibleMoves.addMoveList(knightMoves);

  moveList kingMoves = possibleMovesBKings(game, info);
  possibleMoves.addMoveList(kingMoves);

  moveList rookMoves = possibleMovesBRooks(game, info);
  possibleMoves.addMoveList(rookMoves);

  moveList bishopMoves = possibleMovesBBishops(game, info);
  possibleMoves.addMoveList(bishopMoves);

  moveList queenMoves = possibleMovesBQueens(game, info);
  possibleMoves.addMoveList(queenMoves);

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

  /*if( game->WP & square ) {
    //game->WP ^= 1ULL << (63ULL - (U64)change.start);
    setAllToZero(game, )
    game->WP |= 1ULL << (63ULL - (U64)change.end);
  }
  if( game->WN & square ) {
    game->WN ^= 1ULL << (63ULL - (U64)change.start);
    game->WN |= 1ULL << (63ULL - (U64)change.end);
  }
  if( game->WB & square ) {
    game->WB ^= 1ULL << (63ULL - (U64)change.start);
    game->WB |= 1ULL << (63ULL - (U64)change.end);
  }
  if( game->WR & square ) {
    game->WR ^= 1ULL << (63ULL - (U64)change.start);
    game->WR |= 1ULL << (63ULL - (U64)change.end);
  }
  if( game->WQ & square ) {
    game->WQ ^= 1ULL << (63ULL - (U64)change.start);
    game->WQ |= 1ULL << (63ULL - (U64)change.end);
  }
  if( game->WK & square ) {
    game->WK ^= 1ULL << (63ULL - (U64)change.start);
    game->WK |= 1ULL << (63ULL - (U64)change.end);
  }
  if( game->BP & square ) {
    game->BP ^= 1ULL << (63ULL - (U64)change.start);
    game->BP |= 1ULL << (63ULL - (U64)change.end);
  }
  if( game->BN & square ) {
    game->BN ^= 1ULL << (63ULL - (U64)change.start);
    game->BN |= 1ULL << (63ULL - (U64)change.end);
  }
  if( game->BB & square ) {
    game->BB ^= 1ULL << (63ULL - (U64)change.start);
    game->BB |= 1ULL << (63ULL - (U64)change.end);
  }
  if( game->BR & square ) {
    game->BR ^= 1ULL << (63ULL - (U64)change.start);
    game->BR |= 1ULL << (63ULL - (U64)change.end);
  }
  if( game->BQ & square ) {
    game->BQ ^= 1ULL << (63ULL - (U64)change.start);
    game->BQ |= 1ULL << (63ULL - (U64)change.end);
  }
  if( game->BK & square ) {
    game->BK ^= 1ULL << (63ULL - (U64)change.start);
    game->BK |= 1ULL << (63ULL - (U64)change.end);
  }*/
}

void unapplyMove(bitboards* game, move change) {
  U64 startMask = 1ULL << (63ULL - (U64)change.start);
  U64 endMask = 1ULL << (63ULL - (U64)change.end);

  setAllToZero(game, startMask);

  checkBoardMove(game->WP, endMask, startMask);
  checkBoardMove(game->WN, endMask, startMask);
  checkBoardMove(game->WB, endMask, startMask);
  checkBoardMove(game->WR, endMask, startMask);
  checkBoardMove(game->WQ, endMask, startMask);
  checkBoardMove(game->WK, endMask, startMask);
  checkBoardMove(game->BP, endMask, startMask);
  checkBoardMove(game->BN, endMask, startMask);
  checkBoardMove(game->BB, endMask, startMask);
  checkBoardMove(game->BR, endMask, startMask);
  checkBoardMove(game->BQ, endMask, startMask);
  checkBoardMove(game->BK, endMask, startMask);

  switch(change.special) {
    case 2:
      setAllToZero(game, startMask);
      game->WQ |= startMask;
      break;
    case 3:
      setAllToZero(game, startMask);
      game->WR |= startMask;
      break;
    case 4:
      setAllToZero(game, startMask);
      game->WN |= startMask;
      break;
    case 5:
      setAllToZero(game, startMask);
      game->WB |= startMask;
      break;
    case 6:
      setAllToZero(game, startMask);
      game->BQ |= startMask;
      break;
    case 7:
      setAllToZero(game, startMask);
      game->BR |= startMask;
      break;
    case 8:
      setAllToZero(game, startMask);
      game->BN |= startMask;
      break;
    case 9:
      setAllToZero(game, startMask);
      game->BB |= startMask;
      break;
    case 10:
      setAllToZero(game, startMask >> 8);
      break;
    case 11:
      setAllToZero(game, startMask << 8);
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
