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

#include <iostream>
#include <bitboard.h>
#include <moves.h>
#include <cstdlib> // For random

void printArray(char array[64]) {
  for(int y = 0, i = 0; y < 8; y++) {
    for(int x = 0; x < 8; x++, i++) {
      std::cout << array[i];
    }
    std::cout << std::endl;
  }
}

// A way to manually generate a constant bitboard
void generateConstBitboard() {
  std::cout << std::endl;
  for(int y = 0, i = 0; y < 8; y++) {
    for(int x = 0; x < 8; x++, i++) {
      if(i >= 10) {
        std::cout << i << " ";
      } else {
        std::cout << "0" << i << " ";
      }
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;

  std::cout << "Enter the numbers, enter an out of range number to finish" << std::endl;
  int input;
  char board[64] = {
    ' ',' ',' ',' ',' ',' ',' ',' ',
    ' ',' ',' ',' ',' ',' ',' ',' ',
    ' ',' ',' ',' ',' ',' ',' ',' ',
    ' ',' ',' ',' ',' ',' ',' ',' ',
    ' ',' ',' ',' ',' ',' ',' ',' ',
    ' ',' ',' ',' ',' ',' ',' ',' ',
    ' ',' ',' ',' ',' ',' ',' ',' ',
    ' ',' ',' ',' ',' ',' ',' ',' '
  };
  while(1) {
    std::cin >> input;
    if(input < 0 || input >= 64) {
      break;
    }
    board[input] = 'c';
  }

  bitboards conversionBB;
  U64 answer = conversionBB.arrayToBitboard(board, 'c');
  std::cout << answer << std::endl;
}

void printMovePossibilities(const bitboards originalBoard, const moveList possibleMoves) {
  for(int i = 0; i < possibleMoves.length; i++) {
    std::cout << i << " - (" << (int)possibleMoves.moves[i].start << "," << (int)possibleMoves.moves[i].end << ")" << std::endl;
    bitboards changedBoard = originalBoard;
    applyMove(&changedBoard, possibleMoves.moves[i]);
    changedBoard.printStandardArrayBoard();
  }
}

int rangeRandomAlg2 (int min, int max){
    int n = max - min + 1;
    int remainder = RAND_MAX % n;
    int x;
    do{
        x = rand();
    }while (x >= RAND_MAX - remainder);
    return min + x % n;
}

int main() {
  //generateConstBitboard();

  bitboards testBBs;
  testBBs.initStandardBoard();
  moveList possibleMoves2;

  srand (time(NULL));

  /*possibleMoves2.createMove(8,41);
  possibleMoves2.createMove(52,28);
  possibleMoves2.createMove(11,27);
  possibleMoves2.createMove(50,26);*/
  /*possibleMoves2.createMove(51,42);
  possibleMoves2.createMove(52,34);
  possibleMoves2.createMove(55,46);*/

  /*applyMove(&testBBs,possibleMoves2.moves[0]);
  applyMove(&testBBs,possibleMoves2.moves[1]);
  applyMove(&testBBs,possibleMoves2.moves[2]);*/
  /*applyMove(&testBBs,possibleMoves2.moves[3]);
  applyMove(&testBBs,possibleMoves2.moves[4]);*/

  moveList history;
  //history.createMove(11,27);
  moveList possibleMoves;
  //possibleMoves = possibleMovesW(history, testBBs);
  bitboards tempBoard = testBBs;
  for(int i = 0; i < 5; i++) {
    moveList whiteMoves;
    whiteMoves = possibleMovesW(possibleMoves, tempBoard);
    int selection = rangeRandomAlg2(0, whiteMoves.length - 1);
    possibleMoves.addMove(whiteMoves.moves[selection]);
    applyMove(&tempBoard, whiteMoves.moves[selection]);

    tempBoard.printStandardArrayBoard();
    std::cout << "-----" << std::endl;

    moveList blackMoves;
    blackMoves = possibleMovesB(possibleMoves, tempBoard);
    selection = rangeRandomAlg2(0, blackMoves.length - 1);
    possibleMoves.addMove(blackMoves.moves[selection]);
    applyMove(&tempBoard, blackMoves.moves[selection]);

    tempBoard.printStandardArrayBoard();
    std::cout << "-----" << std::endl;
  }
  //printMovePossibilities(testBBs, possibleMoves);
  return 0;
}
