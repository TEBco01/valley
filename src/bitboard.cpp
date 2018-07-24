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


#define generateBoards // Generates the setup from a constant array instead of using constant integer values

#include <bitboard.h>
#include <iostream>

void bitboards::initStandardBoard() {
//#ifdef generateBoards
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

  /*const char board2[64] = {
    '%','U','b','q','k','b','A','A',
    'U','U','p','p','p','p','A','A',
    'U','U',' ',' ',' ',' ','A','A',
    'Y','Y','Y','Y','Y','Y','Y','Y',
    'X','X','X','X','X','X','X','X',
    'U','U',' ',' ',' ',' ','A','A',
    'U','U','P','P','P','P','A','A',
    'Z','Z','Z','Z','Z','Z','A','A'
  };

  std::cout << BR << std::endl;
  std::cout << "Trouble: " << arrayToBitboard(board2, '%') << std::endl;
  */

  /*
  std::cout << WP << std::endl;
  std::cout << WN << std::endl;
  std::cout << WB << std::endl;
  std::cout << WR << std::endl;
  std::cout << WQ << std::endl;
  std::cout << WK << std::endl;
  std::cout << BP << std::endl;
  std::cout << BN << std::endl;
  std::cout << BB << std::endl;
  std::cout << BR << std::endl;
  std::cout << BQ << std::endl;
  std::cout << BK << std::endl;
  */

/*
  // Precomputed values
  // These are incorrect, actually
  WP = 130560ULL;
  WN = 130692ULL;
  WB = 130764ULL;
  WR = 131022ULL;
  WQ = 131054ULL;
  WK = 131070ULL;
  BP = 33554430ULL;
  BN = 18446744071662731262ULL;
  BB = 18446744072870690814ULL;
  BR = 18446744072904245247ULL;
  BQ = 18446744073441116159ULL;
  BK = 18446744073709551615ULL;
*/
}

//#ifdef generateBoards
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

  // Alternate method devised when debugging
  /*U64 bitboardCopy = bitboard;

  int i = 63;
  U64 tempCopy = 0;
  while(bitboard != 0 && i >= 0) {
    tempCopy = bitboardCopy;
    bitboardCopy >>= 1;
    if(tempCopy != (bitboardCopy << 1) ) {
      array[i] = character;
    }
    i--;
  }*/
}
//#endif

void bitboards::convertToStanardArray(char array[64]) {
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

  convertToStanardArray(array);

  int i = 0;
  for(int y = 0; y < 8; y++) {
    for(int x = 0; x < 8; x++, i++) {
      std::cout << array[i];
    }
    std::cout << std::endl;
  }
}
