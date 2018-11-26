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

moveList possibleMovesArrayWPawns(moveList history, char game[64]) {
  moveList possibleMoves;
  for (int i=8; i < 56; i++) {
    if(game[i] == 'P') {

      if(i > 15) { // Not in range of end of board
        if(game[i - 8] == ' ') { // Is the space in front open
          move newMove;
          newMove.start = i;
          newMove.end = i - 8;
          possibleMoves.addMove(newMove);

          if(i > 47) { // In starting row
            if(game[i - 16] == ' ') { // and clear two spaces ahead
              move newMove2;
              newMove2.start = i;
              newMove2.end = i - 16;
              possibleMoves.addMove(newMove2);
            }
          }

        }

        if(i%8 != 0 && isLowercase(game[i - 9])) { // Diagonal left attack
          move newMove;
          newMove.start = i;
          newMove.end = i - 9;
          possibleMoves.addMove(newMove);
        }
        if(i%8 != 7 && isLowercase(game[i - 7])) { // Diagonal right attack
          move newMove;
          newMove.start = i;
          newMove.end = i - 7;
          possibleMoves.addMove(newMove);
        }
      }
      else { // We are in range of end of board

      }

    }
  }
  return possibleMoves;
}

moveList possibleMovesArrayW(moveList history, char game[64]) {
  moveList possibleMoves;
  moveList pawnMoves = possibleMovesArrayWPawns(history, game);
  for(int i = 0; i < pawnMoves.length; i++) {
    possibleMoves.addMove(pawnMoves.moves[i]);
  }
  return possibleMoves;
}

/*std::string possibleMovesWPawns(std::string history, bitboards game, infoBoards info) {
  // The array technique is very slow, and should be replaced eventually
  // Update: Instead of converting, we're just going to always use an array
  char array[64];
  game.convertToStandardArray(array);
  for (int i=0; i < 64; i++) {
    if(array[i] == 'P') {

    }
  }

 return "";
}

std::string possibleMovesW(std::string history, bitboards game) {
  infoBoards info;
  return possibleMovesWPawns(history, game, info);
}*/

moveList generateMovesFromBitboard(const U64 toBoard) {
  moveList moves;
  for (int i=0; i < 64; i++) {
    if( toBoard & 1ULL << (63ULL - (U64)i) ) {
      moves.createMove(0, i);
    }
  }
  return moves;
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

moveList possibleMovesKnights(const U64 kBoard, const U64 FriendlyPieces) {
  moveList possibleMoves;

  //Left two up one
  U64 l2u1 = ((kBoard & ~(File_A | File_B | Rank_8)) << 10) & ~FriendlyPieces; // Posible trival optimization by precomupting const bitboard combinations
  moveList l2u1List = generateMovesFromBitboard(l2u1);
  for(int i = 0; i < l2u1List.length; i++) {
    l2u1List.moves[i].start = l2u1List.moves[i].end + 10;
  }
  possibleMoves.addMoveList(l2u1List);

  //Right two up one
  U64 r2u1 = ((kBoard & ~(File_G | File_H | Rank_8)) << 6) & ~FriendlyPieces;
  moveList r2u1List = generateMovesFromBitboard(r2u1);
  for(int i = 0; i < l2u1List.length; i++) {
    r2u1List.moves[i].start = r2u1List.moves[i].end + 6;
  }
  possibleMoves.addMoveList(r2u1List);

  //Left one up two
  U64 l1u2 = ((kBoard & ~(File_A | Rank_7 | Rank_8)) << 17) & ~FriendlyPieces;
  moveList l1u2List = generateMovesFromBitboard(l1u2);
  for(int i = 0; i < l1u2List.length; i++) {
    l1u2List.moves[i].start = l1u2List.moves[i].end + 17;
  }
  possibleMoves.addMoveList(l1u2List);

  //Right one up two
  U64 r1u2 = ((kBoard & ~(File_H | Rank_7 | Rank_8)) << 15) & ~FriendlyPieces;
  moveList r1u2List = generateMovesFromBitboard(r1u2);
  for(int i = 0; i < r1u2List.length; i++) {
    r1u2List.moves[i].start = r1u2List.moves[i].end + 15;
  }
  possibleMoves.addMoveList(r1u2List);

  //Left two down one
  U64 l2d1 = ((kBoard & ~(File_A | File_B | Rank_1)) >> 6) & ~FriendlyPieces;
  moveList l2d1List = generateMovesFromBitboard(l2d1);
  for(int i = 0; i < l2d1List.length; i++) {
    l2d1List.moves[i].start = l2d1List.moves[i].end - 6;
  }
  possibleMoves.addMoveList(l2d1List);

  //Right two down one
  U64 r2d1 = ((kBoard & ~(File_G | File_H | Rank_1)) >> 10) & ~FriendlyPieces;
  moveList r2d1List = generateMovesFromBitboard(r2d1);
  for(int i = 0; i < r2d1List.length; i++) {
    r2d1List.moves[i].start = r2d1List.moves[i].end - 10;
  }
  possibleMoves.addMoveList(r2d1List);

  //Left one down two
  U64 l1d2 = ((kBoard & ~(File_A | Rank_1 | Rank_2)) >> 15) & ~FriendlyPieces;
  moveList l1d2List = generateMovesFromBitboard(l1d2);
  for(int i = 0; i < l1d2List.length; i++) {
    l1d2List.moves[i].start = l1d2List.moves[i].end - 15;
  }
  possibleMoves.addMoveList(l1d2List);

  //Right one down two
  U64 r1d2 = ((kBoard & ~(File_H | Rank_1 | Rank_2)) >> 17) & ~FriendlyPieces;
  moveList r1d2List = generateMovesFromBitboard(r1d2);
  for(int i = 0; i < r1d2List.length; i++) {
    r1d2List.moves[i].start = r1d2List.moves[i].end - 17;
  }
  possibleMoves.addMoveList(r1d2List);

  return possibleMoves;
}

moveList possibleMovesWKnights(const bitboards game, const extraBitboardsInfo info) {
  return possibleMovesKnights(game.WN, info.WhitePieces);
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
