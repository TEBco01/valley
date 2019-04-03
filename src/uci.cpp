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
#include <uci.h>

void tellGUI(std::string message) {
  std::cout << message << "\n";
}

std::string hearGUI() {
  std::string returnString;
  getline(std::cin, returnString);
  return returnString;
}

struct engineState {
  bool isUCI = true;
  bool inGame = false;
  game Game;
};

void processUCI() {
  engineState state;

  while (true) {
    std::string input = hearGUI();
    std::string token = getFirstToken(input);

    if(token == "uci") {
      state.isUCI = true;
      tellGUI("id name Valley");
      tellGUI("id author Tyler Behme");
      tellGUI("uciok");
    }
    else if(token == "isready") {
      tellGUI("readyok");
    }
    else if(token == "debug") {

    }
    else if(token == "setoption") {

    }
    else if(token == "register") {

    }
    else if(token == "ucinewgame") {
      state.Game = game();
      state.inGame = true;
    }
    else if(token == "position") {
      state.Game = game();
      state.inGame = true;
      std::string input2 = removeFirstToken(input);
      std::string token2 = getFirstToken(input2);
      if(token2 == "startpos") {
        std::string input3 = removeFirstToken(input2);
        std::string token3 = getFirstToken(input3);

        if(token3 == "moves")
          parseMovesIntoGame(state.Game, removeFirstToken(input3));
      }
      else if(token2 == "fen") {
        std::string input3 = removeFirstToken(input2);
        std::string token3 = getFirstToken(input3);

        state.Game = FENtoGame(input3.c_str());

        std::string input4 = input3;
        while(input4 != "") { // Can be made safer than 'whiling' through
            if(getFirstToken(input4) == "moves") {
              parseMovesIntoGame(state.Game, removeFirstToken(input4));
              break;
            }
          input4 = removeFirstToken(input4);
        }
      }
      else if(token2 == "moves") {
        parseMovesIntoGame(state.Game, removeFirstToken(input2));
      }
    }
    else if(token == "go") { // picks a random legal move
      if(!state.inGame)
      {
        state.Game = game();
        state.inGame = true;
      }

      linkedMoveList moves = state.Game.generateLegalMoves();
      int length = 0;
      moveNode* i = moves.head;
      while(i != NULL) {
        length++;
        i = i->next;
      }
      int moveSelection = rangeRandomAlg2(0, length - 1);
      int j = 0;
      i = moves.head;
      while(i != NULL) {
        if(j == moveSelection) {
          tellGUI("bestmove " + moveToAlgebraic(i->data));
        }
        j++;
        i = i->next;
      }
    }
    else if(token == "stop") {

    }
    else if(token == "ponderhit") {

    }
    else if(token == "quit") {
      return;
    }
    else {
      // Implement checking second token later...
    }
  }
}
