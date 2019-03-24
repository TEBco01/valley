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


#define generateBoards // Generates the setup from a constant array instead of using constant integer values

#include <bitboard.h>
#include <iostream>

void bitboards::initStandardBoard() {
  const char board[64] = {
    'r','n','b','q','k','b','n','r',
    'p','p','p','p','p','p','p','p',
    ' ',' ',' ',' ',' ',' ',' ',' ',
    ' ',' ',' ',' ',' ',' ',' ',' ',
    ' ',' ',' ',' ',' ',' ',' ',' ',
    ' ',' ',' ',' ',' ',' ',' ',' ',
    'P','P','P','P','P','P','P','P',
    'R','N','B','Q','K','B','N','R'
  };

  WP = arrayToBitboard(board, 'P');
  WN = arrayToBitboard(board, 'N');
  WB = arrayToBitboard(board, 'B');
  WR = arrayToBitboard(board, 'R');
  WQ = arrayToBitboard(board, 'Q');
  WK = arrayToBitboard(board, 'K');
  BP = arrayToBitboard(board, 'p');
  BN = arrayToBitboard(board, 'n');
  BB = arrayToBitboard(board, 'b');
  BR = arrayToBitboard(board, 'r');
  BQ = arrayToBitboard(board, 'q');
  BK = arrayToBitboard(board, 'k');
}

U64 bitboards::arrayToBitboard(const char array[64], char character) {
  U64 value = 0;
  for (int i=0; i < 64; i++) {
    if(array[i] == character) {
      value |= 1ULL << (63ULL - (U64)i);
    }
  }
  return value;
}

void bitboards::bitboardToArray(const U64 bitboard, char array[64], char character) {
  for (int i=0; i < 64; i++) {
    if( bitboard & 1ULL << (63ULL - (U64)i) ) {
      array[i] = character;
    }
  }
}

void bitboards::convertToStandardArray(char array[64]) {
  for (int i=0; i < 64; i++) {
    array[i] = ' ';
  }

  bitboardToArray(WP, array, 'P');
  bitboardToArray(WN, array, 'N');
  bitboardToArray(WB, array, 'B');
  bitboardToArray(WR, array, 'R');
  bitboardToArray(WQ, array, 'Q');
  bitboardToArray(WK, array, 'K');
  bitboardToArray(BP, array, 'p');
  bitboardToArray(BN, array, 'n');
  bitboardToArray(BB, array, 'b');
  bitboardToArray(BR, array, 'r');
  bitboardToArray(BQ, array, 'q');
  bitboardToArray(BK, array, 'k');
}

void bitboards::printStandardArrayBoard() {
  char array[64];
  for (int i=0; i < 64; i++) {
    array[i] = ' ';
  }

  convertToStandardArray(array);

  int i = 0;
  for(int y = 0; y < 8; y++) {
    for(int x = 0; x < 8; x++, i++) {
      std::cout << array[i];
    }
    std::cout << std::endl;
  }
}
