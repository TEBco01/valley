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

// For files
#include <iostream>
#include <fstream>
#include <cstring>

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

// Does not check for bounds. Liable for error TODO: Fix
byte algebraicToSquare(const char* squareString) {
  byte returnValue;
  returnValue = squareString[0] - 'a';
  returnValue += 8 * (8 - (squareString[1] - '0'));
  return returnValue;
}

move algebraicToMove(const char* moveString) {
  move returnMove;
  returnMove.start = algebraicToSquare(moveString);
  returnMove.end = algebraicToSquare(moveString + 2);
  return returnMove;
}

U64 Perft(int depth, game Game)
{
    U64 nodes = 0;

    if (depth == 0) return 1;

    linkedMoveList moves = Game.generateLegalMoves();

    moveNode* i = moves.head;
    while(i != NULL) {
      Game.makeMove(i->data);
      nodes += Perft(depth - 1, Game);
      Game.undoMove();
      i = i->next;
    }
    moves.deleteList();
    return nodes;
}

void generateStockfishPerft(linkedMoveList history, int depth) {
  std::ofstream inputFile ("stockfishInput");
  std::string moves;

  moveNode* i = history.head;
  while(i != NULL) {
    moves += " ";
    moves += moveToAlgebraic(i->data);
    i = i->next;
  }

  inputFile << "position startpos moves" << moves << std::endl;
  inputFile << "go perft " << depth << std::endl;
  inputFile.close();
}

linkedMoveList getStockfishMoves() {
  std::ifstream outputFile ("stockfishOutput");

  linkedMoveList returnList;

  std::string line;
  std::getline(outputFile, line);
  while (std::getline(outputFile, line))
  {
      if(line == "")
        break;

      char cString[16];
      strcpy(cString, line.c_str());
      returnList.add(algebraicToMove(cString));
  }
  outputFile.close();
  return returnList;
}

bool movesSame(move a, move b) {
  if(moveToAlgebraic(a) == moveToAlgebraic(b))
    return true;
  return false;
}

struct moveCheck {
  move thisMove;
  int thisCount = 0;
  bool isAccounted = false;
};

bool checkMovesSame(linkedMoveList history, linkedMoveList ourMoves) {
  generateStockfishPerft(history, 1);
  system("cat stockfishInput | stockfish > stockfishOutput");
  linkedMoveList stockfishMoves = getStockfishMoves();
  //stockfishMoves.remove(0);

  int length1 = 0;
  moveNode* i = ourMoves.head;
  while(i != NULL) {
    length1++;
    i = i->next;
  }

  int length2 = 0;
  i = stockfishMoves.head;
  while(i != NULL) {
    length2++;
    i = i->next;
  }

  moveCheck moveChecks1[length1];
  moveCheck moveChecks2[length2];

  i = ourMoves.head;
  int j = 0;
  while(i != NULL) {
    moveChecks1[j].thisMove = i->data;
    j++;
    i = i->next;
  }

  i = stockfishMoves.head;
  j = 0;
  while(i != NULL) {
    moveChecks2[j].thisMove = i->data;
    j++;
    i = i->next;
  }

  for(int j = 0; j < length1; j++) {
    moveCheck curMove1 = moveChecks1[j];
    if(curMove1.isAccounted)
      continue;
    for(int k = 0; k < length2; k++) {
      moveCheck curMove2 = moveChecks2[k];
      if(curMove2.isAccounted)
        continue;
      if(movesSame(curMove1.thisMove, curMove2.thisMove)) {
        moveChecks1[j].isAccounted = true;
        moveChecks2[k].isAccounted = true;
      }
    }
  }

  bool returner = true;

  std::string historyStr;
  i = history.head;
  while(i != NULL) {
    historyStr += moveToAlgebraic(i->data);
    historyStr += " ";
    i = i->next;
  }

  for(int j = 0; j < length1; j++) {
    if(!moveChecks1[j].isAccounted) {
      std::cout << "Extra move from Valley: " << moveToAlgebraic(moveChecks1[j].thisMove) << " Previous moves were: " << historyStr << std::endl;
      returner = false;
    }
  }

  for(int j = 0; j < length2; j++) {
    if(!moveChecks2[j].isAccounted) {
      std::cout << "Missing move from Valley: " << moveToAlgebraic(moveChecks2[j].thisMove) << " Previous moves were: " << historyStr << std::endl;
      returner = false;
    }
  }

  return returner;
}

void compareMoveGeneration(int depth, game Game, linkedMoveList history, int historySize = 0) {
  linkedMoveList moves = Game.generateSemilegalMoves();

  int length = 0;

  moveNode* i = moves.head;
  while(i != NULL) {
    length++;
    i = i->next;
  }

  i = moves.head;
  int j = 0;
  while(i != NULL) {
    Game.makeMove(i->data);
    Game.undoMove();
    j++;
    i = i->next;
  }

  checkMovesSame(history, moves);
  if(depth != 0) {
    moveNode* i = moves.head;
    while(i != NULL) {
      history.add(i->data);
      Game.makeMove(i->data);
      compareMoveGeneration(depth - 1, Game, history, historySize + 1);
      Game.undoMove();
      history.remove(historySize);
      i = i->next;
    }
  }
}

void getStockfishNumbers(int& outLength, moveCheck* moves) {
  //int length = 0;

  /*std::ifstream outputFile ("stockfishOutput");


  std::string line;
  std::getline(outputFile, line);
  while (std::getline(outputFile, line))
  {
      if(line == "")
        break;

      length++;
  }
  outputFile.close();*/

  //moveCheck moves[length];

  std::ifstream outputFile2 ("stockfishOutput");

  int i = 0;
  std::string line2;
  std::getline(outputFile2, line2);
  while (std::getline(outputFile2, line2))
  {
      if(line2 == "")
        break;

      char cString[16];
      strcpy(cString, line2.c_str());

      moves[i].thisCount = atoi(&cString[6]);
      moves[i].thisMove = algebraicToMove(cString);
      i++;
  }
  outputFile2.close();
  outLength = i;
}

void CompareMoveGenerationAdvanced(int depth, game Game, linkedMoveList history, int historySize);

void checkMovesSameAdvancedHelper(linkedMoveList history, moveCheck* moveChecks1, int length, int depth, game Game, int historySize = 0) {
  generateStockfishPerft(history, depth);
  system("cat stockfishInput | stockfish > stockfishOutput");
  int length2 = 0;
  moveCheck moveChecks2 [256];
  getStockfishNumbers(length2, moveChecks2);

  for(int j = 0; j < length; j++) {
    moveCheck curMove1 = moveChecks1[j];
    if(curMove1.isAccounted)
      continue;
    for(int k = 0; k < length2; k++) {
      moveCheck curMove2 = moveChecks2[k];
      if(curMove2.isAccounted)
        continue;
      if(movesSame(curMove1.thisMove, curMove2.thisMove) && curMove1.thisCount == curMove2.thisCount) {
        moveChecks1[j].isAccounted = true;
        moveChecks2[k].isAccounted = true;
      }
    }
  }

  for(int j = 0; j < length; j++) {
    if(!moveChecks1[j].isAccounted) {
      history.add(moveChecks1[j].thisMove);
      Game.makeMove(moveChecks1[j].thisMove);
      CompareMoveGenerationAdvanced(depth - 1, Game, history, historySize + 1);
      Game.undoMove();
      history.remove(historySize);
    }
  }

  for(int j = 0; j < length2; j++) {
    if(!moveChecks2[j].isAccounted) {
      history.add(moveChecks2[j].thisMove);
      Game.makeMove(moveChecks2[j].thisMove);
      CompareMoveGenerationAdvanced(depth - 1, Game, history, historySize + 1);
      Game.undoMove();
      history.remove(historySize);
    }
  }
}

void CompareMoveGenerationAdvanced(int depth, game Game, linkedMoveList history, int historySize = 0) {
  linkedMoveList moves = Game.generateLegalMoves();
  bool same = checkMovesSame(history, moves);

  if(same && depth != 0) {
    int length = 0;
    moveNode* i = moves.head;
    while(i != NULL) {
      length++;
      i = i->next;
    }

    moveCheck moveChecks1[length];

    i = moves.head;
    int j = 0;
    while(i != NULL) {
      Game.makeMove(i->data);
      moveChecks1[j].thisCount = (int)Perft(depth - 1, Game);
      moveChecks1[j].thisMove = i->data;
      Game.undoMove();
      j++;
      i = i->next;
    }

    checkMovesSameAdvancedHelper(history, moveChecks1, length, depth, Game, historySize);
  }
}

int main() {
  game Game;
  move e2e4;
  e2e4.start = 52;
  e2e4.end = 36;
  //Game.makeMove(e2e4);

  linkedMoveList test;
  //test.add(e2e4);

  /*moveNode* i = stockfishMoves.head;
  while(i != NULL) {
    std::cout << moveToAlgebraic(i->data) << std::endl;
    i = i->next;
  }*/

  /*e7e5;
  e7e5.start = 12;
  e7e5.end = 28;
  Game.makeMove(e7e5);*/

  //Game.boards.printStandardArrayBoard();

  //int depth = 2;
  //int number = 0;

  //linkedMoveList moves = Game.generateSemilegalMoves();
  //checkMovesSame(test, moves)
  game Game1;
  linkedMoveList history;
  std::cout << Perft(6,Game1) << std::endl;
  //CompareMoveGenerationAdvanced(5, Game1, history);

  /*i = moves.head;
  while(i != NULL) {
    Game.makeMove(i->data);
    int subnumber = Perft(depth - 1, Game);
    number += subnumber;
    std::cout << moveToAlgebraic(i->data) << ": " << subnumber << std::endl;
    Game.undoMove();
    i = i->next;
  }

  std::cout << std::endl << "Nodes searched: " << number << std::endl;;*/

  return 0;
}
