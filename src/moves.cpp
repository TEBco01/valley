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
    U64 fw1 = ( (game.WP & ~Rank_7) << 8 ) & ~(info.BlackPieces & info.WhitePieces);
  moveList fw1List = generateMovesFromBitboard(fw1);
  for(int i = 0; i < fw1List.length; i++) {
    fw1List.moves[i].start = fw1List.moves[i].end + 8;
  }
  possibleMoves.addMoveList(fw1List);
  // Move forward two
  // Attack
  // Promotion
  // En passant
  return possibleMoves;
}

// Only guaranteed for standard chess
moveList possibleMovesW(moveList history, const bitboards game) {
  moveList possibleMoves;

  extraBitboardsInfo info;
  info.updateFromBitboards(game);

  moveList pawnMoves = possibleMovesWPawns(history, game, info);
  for(int i = 0; i < pawnMoves.length; i++) {
    possibleMoves.addMove(pawnMoves.moves[i]);
  }
  return possibleMoves;
}

// Note: lots of room for optimization; no support for speical moves
void applyMove(bitboards* game, move change) {
  if( game->WP & 1ULL << (63ULL - (U64)change.start) ) {
    game->WP ^= 1ULL << (63ULL - (U64)change.start);
    game->WP |= 1ULL << (63ULL - (U64)change.end);
  }
  if( game->WN & 1ULL << (63ULL - (U64)change.start) ) {
    game->WN ^= 1ULL << (63ULL - (U64)change.start);
    game->WN |= 1ULL << (63ULL - (U64)change.end);
  }
  if( game->WB & 1ULL << (63ULL - (U64)change.start) ) {
    game->WB ^= 1ULL << (63ULL - (U64)change.start);
    game->WB |= 1ULL << (63ULL - (U64)change.end);
  }
  if( game->WR & 1ULL << (63ULL - (U64)change.start) ) {
    game->WR ^= 1ULL << (63ULL - (U64)change.start);
    game->WR |= 1ULL << (63ULL - (U64)change.end);
  }
  if( game->WQ & 1ULL << (63ULL - (U64)change.start) ) {
    game->WQ ^= 1ULL << (63ULL - (U64)change.start);
    game->WQ |= 1ULL << (63ULL - (U64)change.end);
  }
  if( game->WK & 1ULL << (63ULL - (U64)change.start) ) {
    game->WK ^= 1ULL << (63ULL - (U64)change.start);
    game->WK |= 1ULL << (63ULL - (U64)change.end);
  }
  if( game->BP & 1ULL << (63ULL - (U64)change.start) ) {
    game->BP ^= 1ULL << (63ULL - (U64)change.start);
    game->BP |= 1ULL << (63ULL - (U64)change.end);
  }
  if( game->BN & 1ULL << (63ULL - (U64)change.start) ) {
    game->BN ^= 1ULL << (63ULL - (U64)change.start);
    game->BN |= 1ULL << (63ULL - (U64)change.end);
  }
  if( game->BB & 1ULL << (63ULL - (U64)change.start) ) {
    game->BB ^= 1ULL << (63ULL - (U64)change.start);
    game->BB |= 1ULL << (63ULL - (U64)change.end);
  }
  if( game->BR & 1ULL << (63ULL - (U64)change.start) ) {
    game->BR ^= 1ULL << (63ULL - (U64)change.start);
    game->BR |= 1ULL << (63ULL - (U64)change.end);
  }
  if( game->BQ & 1ULL << (63ULL - (U64)change.start) ) {
    game->BQ ^= 1ULL << (63ULL - (U64)change.start);
    game->BQ |= 1ULL << (63ULL - (U64)change.end);
  }
  if( game->BK & 1ULL << (63ULL - (U64)change.start) ) {
    game->BK ^= 1ULL << (63ULL - (U64)change.start);
    game->BK |= 1ULL << (63ULL - (U64)change.end);
  }
}
