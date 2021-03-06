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
#include <cstddef>

typedef unsigned char byte;

C64 Rank_8 = 18374686479671623680ULL;
C64 Rank_7 = 71776119061217280ULL;
C64 Rank_5 = 1095216660480ULL;
C64 Rank_4 = 4278190080ULL;
C64 Rank_2 = 65280ULL;
C64 Rank_1 = 255ULL;
C64 File_A = 9259542123273814144ULL;
C64 File_B = 4629771061636907072ULL;
C64 File_G = 144680345676153346ULL;
C64 File_H = 72340172838076673ULL;
enum special : byte {
	castle = 1, promotionWQ = 2, promotionWR = 3, promotionWK = 4, promotionWB = 5, promotionBQ = 6, promotionBR = 7, promotionBK = 8, promotionBB = 9, enPassantW = 10, enPassantB = 11
};

struct move {
  byte start = 0, end = 0, special = 0; // For special: 1 is castle, 2 is pawn into white Queen, 3 is pawn into white Rook, 4 is pawn into white Knight, 5 is pawn into white Bishop; black pawns are the same except + 4; 10 is white en passant; 11 is black en passant
};

struct extraBitboardsInfo {
  U64 WhitePieces;
  U64 BlackPieces;

  void updateFromBitboards(const bitboards game) {
    WhitePieces = game.WP | game.WN | game.WB | game.WR | game.WQ | game.WK;
    BlackPieces = game.BP | game.BN | game.BB | game.BR | game.BQ | game.BK;
  }
};

class arrayMoveList {
	public:
		void add(move value); // Adds a move to the list
		void create(byte start, byte end, byte special); // Creates and adds a move to the list
    void create(byte start, byte end) {this->create(start, end, 0);}
		bool next(); // Moves the iterator to the next move. Returns false if there are no more moves
		move getMove(); // Returns the move the iterator is currently on
		void remove(); // Removes the move the iterator is currently on
		void resetIterator(); // Sets the iterator back to the beginning
	private:
		move moves[256]; // This implementation has no way to deal with more than 256 moves in a list :) This can be fixed later
		int stackCount = 0;
		int iteratorCount = -1;
};

struct moveNode {
	move data;
	moveNode *next;
};

class linkedMoveList {
	public:
		linkedMoveList() {
			head = NULL;
			tail = NULL;
		};
		void add(move value);
		void remove(int pos);
		void create(byte start, byte end, byte special);
    void create(byte start, byte end) {this->create(start, end, 0);}
		void operator=(linkedMoveList b);
		void operator+=(linkedMoveList b);
    void deleteList(); // Manual deconstruction
		moveNode *head, *tail;
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

  void createMove(byte start, byte end, byte special = 0) {
    move newMove;
    newMove.start = start;
    newMove.end = end;
    newMove.special = special;
    this->addMove(newMove);
  }

  void addMoveList(moveList list) {
    for(int i = 0; i < list.length; i++) {
      this->addMove(list.moves[i]);
    }
  }

  void removeMove(int moveNumber) {
    length--;
    move* temp = moves;
    moves = new move[length];
    for(int i = 0; i < (length + 1); i++) {
      if(i < moveNumber) {
        moves[i] = temp[i];
      } else if (i > moveNumber) {
        moves[i] = temp[i-1];
      }
    }
    delete [] temp;
  }
};
