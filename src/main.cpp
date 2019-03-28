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
#include <moves.h>
#include <game.h>
#include <utilities.h>

// For files
#include <iostream>
#include <fstream>
#include <cstring>

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
