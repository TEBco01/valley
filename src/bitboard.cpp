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


//#define generateBoards // Generates the setup from a constant array instead of using constant integer values

#include <bitboard.h>
//#include <iostream>

void bitboards::initStandardBoard() {
#ifdef generateBoards
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

#else
  // Precomputed values
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
#endif
}

#ifdef generateBoards
U64 bitboards::arrayToBitboard(const char array[64], char character) {
  U64 value;
  for (int i=0; i < 64; i++) {
    if(array[i] == character) {
      value |= 1 << (64 - i);
    }
  }
  return value;
}
#endif
