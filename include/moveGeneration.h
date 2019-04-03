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
#pragma once

#include <bitboard.h>
#include <moves.h>

linkedMoveList possibleMovesW(move lastMove, const bitboards game);
linkedMoveList possibleMovesB(move lastMove, const bitboards game);

void applyMove(bitboards* game, move change);
void unapplyMove(bitboards* game, move change);

bool attackOnKing(move change, const bitboards game);
bool missingKing(const bitboards game);
