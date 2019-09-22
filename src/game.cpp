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

#include <game.h>

arrayMoveList game::generateLegalMoves() {
    arrayMoveList moves = generateSemilegalMoves();
    arrayMoveList returnedMoves;

    moves.resetIterator();
    while(moves.next()) {
        if(moves.getMove().special == 1) { // Are castling kings put in check en route?
            bool good = true;
            makeMove(moves.getMove());
            if(!isGameLegal()) good = false;
            undoMove();

            if(inCheck(blacksTurn, boards)) good = false;

            move intermediate;
            switch (moves.getMove().end) {
                case 2:
                    intermediate.start = moves.getMove().start;
                    intermediate.end = 3;
                    makeMove(intermediate);
                    if(!isGameLegal()) good = false;
                    undoMove();
                    break;
                case 6:
                    intermediate.start = moves.getMove().start;
                    intermediate.end = 5;
                    makeMove(intermediate);
                    if(!isGameLegal()) good = false;
                    undoMove();
                    break;
                case 58:
                    intermediate.start = moves.getMove().start;
                    intermediate.end = 59;
                    makeMove(intermediate);
                    if(!isGameLegal()) good = false;
                    undoMove();
                    break;
                case 62:
                    intermediate.start = moves.getMove().start;
                    intermediate.end = 61;
                    makeMove(intermediate);
                    if(!isGameLegal()) good = false;
                    undoMove();
                    break;
            }
            if(good) returnedMoves.add(moves.getMove());
        } else {
            makeMove(moves.getMove());
            if(isGameLegal()) returnedMoves.add(moves.getMove());
            undoMove();
        }
    }
    return returnedMoves;
}

arrayMoveList game::generateSemilegalMoves() {
    move lastMove;
    if(!history.empty()) {
        lastMove = history.top();
    }

    if(blacksTurn) {
        return possibleMovesB(lastMove, boards);
    } else {
        return possibleMovesW(lastMove, boards);
    }
}

bool game::isGameLegal() { // Most of this function can be moved to or shared with the "inCheck" function
    arrayMoveList moves = generateSemilegalMoves(); // TODO: Big optimization by reusing these generated moves if the depth increases

    moves.resetIterator();
    while(moves.next()) {
        if(attackOnKing(moves.getMove(), boards))
        {
            return false;
        }
    }
    return true;
}

void game::undoMove() {
    boards = boardHistory.top();
    boardHistory.pop();
    history.pop();
    blacksTurn = !blacksTurn;
}

void game::makeMove(move moveMade) {
    boardHistory.push(boards);
    applyMove(&boards, moveMade);
    history.push(moveMade);
    blacksTurn = !blacksTurn;
}
