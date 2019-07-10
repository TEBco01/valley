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

#include <moveGeneration.h>

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

bool pieceAtSquare(const U64 board, const int square) {
  if( board & 1ULL << (63ULL - (U64)square) ) {
    return true;
  } else {
    return false;
  }
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
  U64 atL = ((game.WP & ~File_A & ~Rank_7) << 9) & info.BlackPieces;
  constantShiftGenerator(atL, 9, possibleMoves);

  U64 atR = ((game.WP & ~File_H & ~Rank_7) << 7) & info.BlackPieces;
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

  U64 prL = ((game.WP & Rank_7) << 9) & ~(info.WhitePieces) & info.BlackPieces;
  linkedMoveList prLList = generateMovesFromBitboard(prL);
  linkedMoveList prLExtraList;
  i = prLList.head;
  while(i != NULL) {
    i->data.start = i->data.end + 9;
    i->data.special = 2;
    for(byte j = 3; j <= 5; j++) {
      prLExtraList.create(i->data.start, i->data.end, j);
    }
    i = i->next;
  }
  possibleMoves += prLList;
  possibleMoves += prLExtraList;

  U64 prR = ((game.WP & Rank_7) << 7) & ~(info.WhitePieces) & info.BlackPieces;
  linkedMoveList prRList = generateMovesFromBitboard(prR);
  linkedMoveList prRExtraList;
  i = prRList.head;
  while(i != NULL) {
    i->data.start = i->data.end + 7;
    i->data.special = 2;
    for(byte j = 3; j <= 5; j++) {
      prRExtraList.create(i->data.start, i->data.end, j);
    }
    i = i->next;
  }
  possibleMoves += prRList;
  possibleMoves += prRExtraList;

  // En passant
	if(!(lastMove.start == 0 && lastMove.end == 0)) { // Was there a last move?
		if(24 <= lastMove.end && lastMove.end <= 31) { // Did it end in our range?
			if(pieceAtSquare(game.BP, lastMove.end) && 8 <= lastMove.start && lastMove.start <= 15) { // Was it an enemy pawn? Did this pawn move two?
				if(lastMove.end >= 25) // Was it in bounds on the left?
					if(pieceAtSquare(game.WP, lastMove.end - 1)) // Is there a friendly piece on the left side?
						possibleMoves.create(lastMove.end - 1, lastMove.end - 8, 10);
				if(lastMove.end <= 30) // Was it in bounds on the right?
					if(pieceAtSquare(game.WP, lastMove.end + 1)) // Is there a friendly piece on the right side?
						possibleMoves.create(lastMove.end + 1, lastMove.end - 8, 10);
			}
		}
	}

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
  U64 atL = ((game.BP & ~File_H & ~Rank_2) >> 9) & info.WhitePieces;
  constantShiftGenerator(atL, -9, possibleMoves);

  U64 atR = ((game.BP & ~File_A & ~Rank_2) >> 7) & info.WhitePieces;
  constantShiftGenerator(atR, -7, possibleMoves);

  // Promotion
  U64 pr = ((game.BP & Rank_2) >> 8) & ~(info.BlackPieces | info.WhitePieces);
  linkedMoveList prList = generateMovesFromBitboard(pr);
  linkedMoveList prExtraList;
  moveNode* i = prList.head;
  while(i != NULL) {
    i->data.start = i->data.end - 8;
    i->data.special = 6;
    for(byte j = 7; j <= 9; j++) {
      prExtraList.create(i->data.start, i->data.end, j);
    }
    i = i->next;
  }
  possibleMoves += prList;
  possibleMoves += prExtraList;

  U64 prL = ((game.BP & Rank_2) >> 7) & ~(info.BlackPieces) & info.WhitePieces;
  linkedMoveList prLList = generateMovesFromBitboard(prL);
  linkedMoveList prLExtraList;
  i = prLList.head;
  while(i != NULL) {
    i->data.start = i->data.end - 7;
    i->data.special = 6;
    for(byte j = 7; j <= 9; j++) {
      prLExtraList.create(i->data.start, i->data.end, j);
    }
    i = i->next;
  }
  possibleMoves += prLList;
  possibleMoves += prLExtraList;

  U64 prR = ((game.BP & Rank_2) >> 9) & ~(info.BlackPieces) & info.WhitePieces;
  linkedMoveList prRList = generateMovesFromBitboard(prR);
  linkedMoveList prRExtraList;
  i = prRList.head;
  while(i != NULL) {
    i->data.start = i->data.end - 9;
    i->data.special = 6;
    for(byte j = 7; j <= 9; j++) {
      prRExtraList.create(i->data.start, i->data.end, j);
    }
    i = i->next;
  }
  possibleMoves += prRList;
  possibleMoves += prRExtraList;

  // En passant
	if(!(lastMove.start == 0 && lastMove.end == 0)) { // Was there a last move?
		if(32 <= lastMove.end && lastMove.end <= 39) { // Did it end in our range?
			if(pieceAtSquare(game.WP, lastMove.end)  && 48 <= lastMove.start && lastMove.start <= 55) { // Was it an enemy pawn? Did this pawn move two?
				if(lastMove.end >= 33) // Was it in bounds on the left?
					if(pieceAtSquare(game.BP, lastMove.end - 1)) // Is there a friendly piece on the left side?
						possibleMoves.create(lastMove.end - 1, lastMove.end + 8, 11);
				if(lastMove.end <= 38) // Was it in bounds on the right?
					if(pieceAtSquare(game.BP, lastMove.end + 1)) // Is there a friendly piece on the right side?
						possibleMoves.create(lastMove.end + 1, lastMove.end + 8, 11);
			}
		}
	}

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

  U64 u = ((kBoard & ~(Rank_8)) << 8) & ~FriendlyPieces;
  constantShiftGenerator(u, 8, possibleMoves);

  U64 ur = ((kBoard & ~(File_H | Rank_8)) << 7) & ~FriendlyPieces;
  constantShiftGenerator(ur, 7, possibleMoves);

  U64 ul = ((kBoard & ~(File_A | Rank_8)) << 9) & ~FriendlyPieces;
  constantShiftGenerator(ul, 9, possibleMoves);

  U64 l = ((kBoard & ~(File_A)) << 1) & ~FriendlyPieces;
  constantShiftGenerator(l, 1, possibleMoves);

  U64 r = ((kBoard & ~(File_H)) >> 1) & ~FriendlyPieces;
  constantShiftGenerator(r, -1, possibleMoves);

  U64 d = ((kBoard & ~(Rank_1)) >> 8) & ~FriendlyPieces;
  constantShiftGenerator(d, -8, possibleMoves);

  U64 dr = ((kBoard & ~(File_H | Rank_1)) >> 9) & ~FriendlyPieces;
  constantShiftGenerator(dr, -9, possibleMoves);

  U64 dl = ((kBoard & ~(File_A | Rank_1)) >> 7) & ~FriendlyPieces;
  constantShiftGenerator(dl, -7, possibleMoves);

  return possibleMoves;
}

linkedMoveList possibleMovesWKings(const bitboards game, const extraBitboardsInfo info) {
  return possibleMovesKings(game.WK, info.WhitePieces);
}

linkedMoveList possibleMovesBKings(const bitboards game, const extraBitboardsInfo info) {
  return possibleMovesKings(game.BK, info.BlackPieces);
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

linkedMoveList possibleMovesRooks(const U64 rBoard, const U64 FriendlyPieces, const U64 EnemyPieces, castleBools castleInfo, bool black) {
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

	if(black) {
		if(castleInfo.blackACan) {
			if(!(8070450532247928832ULL & (FriendlyPieces | EnemyPieces) )) // Checks for pieces between the rook and king
				possibleMoves.create(4, 2, 1);
		}
		if(castleInfo.blackHCan) {
			if(!(432345564227567616ULL & (FriendlyPieces | EnemyPieces) ))
				possibleMoves.create(4, 6, 1);
		}
	} else {
		if(castleInfo.whiteACan) {
			if(!(112ULL & (FriendlyPieces | EnemyPieces) ))
				possibleMoves.create(60, 58, 1);
		}
		if(castleInfo.whiteHCan) {
			if(!(6ULL & (FriendlyPieces | EnemyPieces) ))
				possibleMoves.create(60, 62, 1);
		}
	}

  return possibleMoves;
}

linkedMoveList possibleMovesWRooks(const bitboards game, const extraBitboardsInfo info) {
  return possibleMovesRooks(game.WR, info.WhitePieces, info.BlackPieces, game.castleInfo, false);
}

linkedMoveList possibleMovesBRooks(const bitboards game, const extraBitboardsInfo info) {
  return possibleMovesRooks(game.BR, info.BlackPieces, info.WhitePieces, game.castleInfo, true);
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

// List all moves the player can make for the given game
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

bool checkBoardMove(U64& board, U64 startMask, U64 endMask) { // TODO: possible optimization by making returnless version
  if( board & startMask ) {
    board &= ~startMask;
    board |= endMask;
		return true;
  }
	return false;
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
  if(checkBoardMove(game->WK, startMask, endMask)) { // TODO: Posible optimization by checking position number instead
		game->castleInfo.whiteACan = false;
		game->castleInfo.whiteHCan = false;
	}
  checkBoardMove(game->BP, startMask, endMask);
  checkBoardMove(game->BN, startMask, endMask);
  checkBoardMove(game->BB, startMask, endMask);
  checkBoardMove(game->BR, startMask, endMask);
  checkBoardMove(game->BQ, startMask, endMask);
  if(checkBoardMove(game->BK, startMask, endMask)) {
		game->castleInfo.blackACan = false;
		game->castleInfo.blackHCan = false;
	}

	if(change.start == 63) {
		game->castleInfo.whiteHCan = false;
	} else if(change.start == 56) {
		game->castleInfo.whiteACan = false;
	} else if(change.start == 0) {
		game->castleInfo.blackACan = false;
	} else if(change.start == 7)
		game->castleInfo.blackHCan = false;

  switch(change.special) { // Check moves.h for the list of special moves. TODO: use an enum instead of a manual code
		case 1:
			if(change.start < 8) {
        //game->castleInfo.blackACan = false;
        //game->castleInfo.blackHCan = false;
				if(change.end == 2) {
          setAllToZero(game, 1ULL << (63ULL - (U64)0));
					game->BR |= 1ULL << (63ULL - (U64)3);
				} else {
          setAllToZero(game, 1ULL << (63ULL - (U64)7));
					game->BR |= 1ULL << (63ULL - (U64)5);
				}
			} else {
        //game->castleInfo.whiteACan = false;
        //game->castleInfo.whiteHCan = false;
				if(change.end == 58) {
          setAllToZero(game, 1ULL << (63ULL - (U64)56));
					game->WR |= 1ULL << (63ULL - (U64)59);
				} else {
          setAllToZero(game, 1ULL << (63ULL - (U64)63));
					game->WR |= 1ULL << (63ULL - (U64)61);
				}
			}
			break;
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

bool missingKing(const bitboards game) {
  if(game.WK && game.BK) {
    return false;
  }
  return true;
}
