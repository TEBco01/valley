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

int main() {
  game Game;

  int depth = 3;
  int number = 0;

  linkedMoveList moves = Game.generateSemilegalMoves();
  moveNode* i = moves.head;
  while(i != NULL) {
    Game.makeMove(i->data);
    int subnumber = Perft(depth - 1, Game);
    number += subnumber;
    std::cout << moveToAlgebraic(i->data) << ": " << subnumber << std::endl;
    Game.undoMove();
    i = i->next;
  }

  std::cout << std::endl << "Nodes searched: " << number << std::endl;;

  return 0;
}
