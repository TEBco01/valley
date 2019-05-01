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

#include <xxhash.h>
#include <moves.h>
#include <iostream>

struct TT {
  U64 hashWP[64];
  U64 hashWN[64];
  U64 hashWB[64];
  U64 hashWR[64];
  U64 hashWQ[64];
  U64 hashWK[64];
  U64 hashBP[64];
  U64 hashBN[64];
  U64 hashBB[64];
  U64 hashBR[64];
  U64 hashBQ[64];
  U64 hashBK[64];
  U64 hashSide;
  U64 hashWhiteA;
  U64 hashWhiteH;
  U64 hashBlackA;
  U64 hashBlackH;
  U64 hashEnPassant[8];

  TT() {
    init(0ULL); // TODO: Search for a better key
    //std::cout << hashWP[0] << std::endl;
  }

  void init(U64 seed);
};
