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

#include <evaluator.h>

int boardCount(const U64 board) {
  if(board == 0) return 0;

  int count = 0;
  for (int i=0; i < 64; i++) {
    if( board & 1ULL << (63ULL - (U64)i) ) {
      count++;
    }
  }
  return count;
}

double pieceCountScore(const game Game) {
  double score = 0.0;

  score += 100.0 * (double)boardCount(Game.boards.WK);
  score += 9.0 * (double)boardCount(Game.boards.WQ);
  score += 3.0 * (double)boardCount(Game.boards.WB);
  score += 3.0 * (double)boardCount(Game.boards.WN);
  score += 5.0 * (double)boardCount(Game.boards.WR);
  score += 1.0 * (double)boardCount(Game.boards.WP);

  score -= 100.0 * (double)boardCount(Game.boards.BK);
  score -= 9.0 * (double)boardCount(Game.boards.BQ);
  score -= 3.0 * (double)boardCount(Game.boards.BB);
  score -= 3.0 * (double)boardCount(Game.boards.BN);
  score -= 5.0 * (double)boardCount(Game.boards.BR);
  score -= 1.0 * (double)boardCount(Game.boards.BP);

  return score;
}

double positionStrengthFast(const game Game) {
  return pieceCountScore(Game);
}

move maxPositionStrengthFast(linkedMoveList moves, game Game) {
  double bestScore = -1.0;
  move bestMove;

  moveNode* i = moves.head;
  while(i != NULL) {
    Game.makeMove(i->data);
    double score = positionStrengthFast(Game);
    Game.undoMove();
    if(score > bestScore) {
      bestScore = score;
      bestMove = i->data;
    }
    i = i->next;
  }
  return bestMove;
}

move minPositionStrengthFast(linkedMoveList moves, game Game) {
  double bestScore = 1e99;
  move bestMove;

  moveNode* i = moves.head;
  while(i != NULL) {
    Game.makeMove(i->data);
    double score = positionStrengthFast(Game);
    Game.undoMove();
    if(score < bestScore) {
      bestScore = score;
      bestMove = i->data;
    }
    i = i->next;
  }
  return bestMove;
}

flexScore negaMax(game Game, int depth) {
    if(depth == 0) {
      double side;
      if(Game.blacksTurn) {
        side = -1.0;
      } else {
        side = 1.0;
      }

      flexScore returner;
      returner.score = side*positionStrengthFast(Game);
      return returner;
    }
    flexScore max;
    linkedMoveList moves = Game.generateLegalMoves();
    for(moveNode* i = moves.head; i != NULL; i = i->next) {
      Game.makeMove(i->data);
      flexScore score = -negaMax(Game, depth - 1);
      Game.undoMove();
      if(score > max) {
        max = score;
      }
    }
    return max;
}

/*void evaluate(game Game, move& bestMove) {
  //bestMove = move;
  bool forBlack = Game.blacksTurn;
  linkedMoveList moves = Game.generateLegalMoves();
  if(!forBlack) {
    bestMove = maxPositionStrengthFast(moves, Game);
  } else {
    bestMove = maxPositionStrengthFast(moves, Game);
  }
}*/

void evaluate(game Game, move& bestMove) {
  flexScore bestScore;

  linkedMoveList moves = Game.generateLegalMoves();
  for(moveNode* i = moves.head; i != NULL; i = i->next) {
    Game.makeMove(i->data);
    flexScore score = negaMax(Game, 3);
    if(score > bestScore) {
      bestScore = score;
      bestMove = i->data;
      //std::cout << moveToAlgebraic(bestMove) << std::endl;
    }
    Game.undoMove();
    //std::cout << moveToAlgebraic(i->data) << " - " << score.score << std::endl;
  }
}
