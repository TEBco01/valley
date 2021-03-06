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
  #if defined(__GNUC__) || defined(__GNUG__)
    return __builtin_popcountll(board);
  #else
    U64 x = board;
    int count = 0;
    while(x) {
      count++;
      x &= x - 1;
    }
    return count;
  #endif
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

flexScore negaMax(game Game, int depth, flexScore alpha, flexScore beta) {
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

    arrayMoveList moves = Game.generateLegalMoves();
    moves.resetIterator();
    if(!moves.next()) {
      flexScore returner;
      returner.tethered = true;
      returner.count = 0;
      returner.draw = true; // We currently assume this is a draw
      return returner;
    }
    moves.resetIterator();
    while(moves.next()) {
      Game.makeMove(moves.getMove());
      flexScore score = -negaMax(Game, depth - 1, --beta, --alpha);
      Game.undoMove();
      if(score >= beta) {
        return beta;
      }
      if(score > alpha)
        alpha = score;
    }

    return alpha;
}

void evaluate(game Game, move& bestMove) {
  flexScore bestScore;
  bestScore.extreme = true;
  bestScore.favorable = false;
  flexScore alpha;
  alpha.extreme = true;
  alpha.favorable = false;
  flexScore beta;
  beta.extreme = true;
  beta.favorable = true;

  arrayMoveList moves = Game.generateLegalMoves();
  moves.resetIterator();
  while(moves.next()) {
    Game.makeMove(moves.getMove());
    flexScore score = negaMax(Game, 3, alpha, beta);
    if(score > bestScore) {
      bestScore = score;
      bestMove = moves.getMove();
    }
    Game.undoMove();
  }
}
