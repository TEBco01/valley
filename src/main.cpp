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

#include <iostream>
#include <bitboard.h>
#include <moves.h>
#include <cstdlib> // For random
#include <game.h>

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

std::string squareToAlgebraic(const byte square) {
  std::string first;
  std::string second;
  switch(square % 8) {
    case 0:
      first = "a";
      break;
    case 1:
      first = "b";
      break;
    case 2:
      first = "c";
      break;
    case 3:
      first = "d";
      break;
    case 4:
      first = "e";
      break;
    case 5:
      first = "f";
      break;
    case 6:
      first = "g";
      break;
    case 7:
      first = "h";
      break;
  }

  switch(square / 8) {
    case 0:
      second = "8";
      break;
    case 1:
      second = "7";
      break;
    case 2:
      second = "6";
      break;
    case 3:
      second = "5";
      break;
    case 4:
      second = "4";
      break;
    case 5:
      second = "3";
      break;
    case 6:
      second = "2";
      break;
    case 7:
      second = "1";
      break;
  }

  return first + second;
}

std::string moveToAlgebraic(const move change) {
  return squareToAlgebraic(change.start) + squareToAlgebraic(change.end);
}

U64 Perft(int depth, game Game)
{
    U64 nodes = 0;

    if (depth == 0) return 1;

    linkedMoveList moves = Game.generateSemilegalMoves();

    moveNode* i = moves.head;
    while(i != NULL) {
      Game.makeMove(i->data);
      nodes += Perft(depth - 1, Game);
      Game.undoMove();
      i = i->next;
    }
    return nodes;
}

int main() {
  game Game;
  std::cout << Perft(4, Game) << std::endl;;

  return 0;
}
