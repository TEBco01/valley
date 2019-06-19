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

#include <utilities.h>

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
  std::string extra = "";
  if(change.special >= 2 && change.special <= 9) {
    switch((change.special - 1) % 4) {
      case 1:
        extra = "q";
        break;
      case 2:
        extra = "r";
        break;
      case 3:
        extra = "k";
        break;
      default:
        extra = "b";
        break;
    }
  }
  return squareToAlgebraic(change.start) + squareToAlgebraic(change.end) + extra;
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

bitboards FENboardToBitboards(const char* board, const int length) {
  char array[64];
  int j = 0;
  for(int i = 0; i < length; i++) {
    if(board[i] == '/')
      continue;
    if(board[i] - '1' >= 0 && board[i] - '8' <= 0) {
      int blanks = board[i] - '0';
      for(int k = j; k < j + blanks; k++)
      {
        array[k] = ' ';
      }
      j += blanks;
    }
    else {
      array[j] = board[i];
      j++;
    }
  }

  bitboards returnBoards;
  returnBoards.standardArrayToBitboards(array);
  return returnBoards;
}

game FENtoGame(const char* FEN) {
  bitboards gameBoards;
  int sectionLength = 0;
  for(int i = 0; i < 64 + 7; i++) {
    if(FEN[i] == ' ')
      break;
    sectionLength++;
  }
  gameBoards = FENboardToBitboards(FEN, sectionLength);
  game returnGame(gameBoards);
  char sideToMove = FEN[sectionLength + 1];
  if(sideToMove == 'b') {
    returnGame.blacksTurn = true;
  }
  int index = sectionLength + 3;
  sectionLength = 0;

  returnGame.boards.castleInfo.whiteACan = false;
  returnGame.boards.castleInfo.whiteHCan = false;
  returnGame.boards.castleInfo.blackACan = false;
  returnGame.boards.castleInfo.blackHCan = false;

  if(index == '-') {
    sectionLength = 1;
  } else {
    for(int i = index; i < index + 6; i++) {
      if(FEN[i] == ' ')
        break;
      sectionLength++;
    }
    for(int i = index; i < index + sectionLength; i++) {
      if(FEN[i] == 'Q')
        returnGame.boards.castleInfo.whiteACan = true;
    }
    for(int i = index; i < index + sectionLength; i++) {
      if(FEN[i] == 'K')
        returnGame.boards.castleInfo.whiteHCan = true;
    }
    for(int i = index; i < index + sectionLength; i++) {
      if(FEN[i] == 'q')
        returnGame.boards.castleInfo.blackACan = true;
    }
    for(int i = index; i < index + sectionLength; i++) {
      if(FEN[i] == 'k')
        returnGame.boards.castleInfo.blackHCan = true;
    }
  }
  index += sectionLength + 1;
  if(FEN[index] != '-') {
    move enPassant;
    enPassant.end = algebraicToSquare(&FEN[index]);
    if(enPassant.end < 32) {
      enPassant.start = enPassant.end - 16;
      enPassant.special = 11;
    } else {
      enPassant.start = enPassant.end + 16;
      enPassant.special = 10;
    }
    returnGame.history.push(enPassant);
    index += 3;
  } else {
    index += 2;
  }

  return returnGame;
}

std::string getFirstToken(std::string input) {
  std::string returnString;
  for(int i = 0; input[i] != ' ' && input[i] != '\0' && input[i] != '\t'; i++) {
    returnString += input[i];
  }
  return returnString;
}

std::string removeFirstToken(std::string input) {
  std::string returnString;
  int i;
  for(i = 0; input[i] != ' ' && input[i] != '\0' && input[i] != '\t'; i++) {}
  for(; (input[i] == ' ' || input[i] == '\t') && input[i] != '\0'; i++) {}
  for(; input[i] != '\0'; i++) {
    returnString += input[i];
  }
  return returnString;
}

linkedMoveList parseMoveString(std::string movesList) {
  linkedMoveList returnList;
  std::string editList = movesList;
  while(editList != "") {
    returnList.add(algebraicToMove(getFirstToken(editList).c_str()));
    editList = removeFirstToken(editList);
  }
  return returnList;
}

void parseMovesIntoGame(game& Game, std::string movesList) {
  linkedMoveList moves = parseMoveString(movesList);

  moveNode* i = moves.head;
  while(i != NULL) {
    Game.makeMove(i->data);
    i = i->next;
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

int rangeRandomAlg2 (int min, int max){
    int n = max - min + 1;
    int remainder = RAND_MAX % n;
    int x;
    do{
        x = rand();
    }while (x >= RAND_MAX - remainder);
    return min + x % n;
}

void printArray(char array[64]) {
  for(int y = 0, i = 0; y < 8; y++) {
    for(int x = 0; x < 8; x++, i++) {
      std::cout << array[i];
    }
    std::cout << std::endl;
  }
}

void printMovePossibilities(const bitboards originalBoard, const moveList possibleMoves) {
  for(int i = 0; i < possibleMoves.length; i++) {
    std::cout << i << " - (" << (int)possibleMoves.moves[i].start << "," << (int)possibleMoves.moves[i].end << ")" << std::endl;
    bitboards changedBoard = originalBoard;
    applyMove(&changedBoard, possibleMoves.moves[i]);
    changedBoard.printStandardArrayBoard();
  }
}
