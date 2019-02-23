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

//typedef unsigned __int64 U64;
typedef unsigned long long U64;
typedef const unsigned long long C64;
//#define C64(constantU64) constantU64##ULL

struct castleBools {
  bool whiteACan = true, whiteHCan = true, blackACan = true, blackHCan = true;
};

struct bitboards {
  U64 WP=0,WN=0,WB=0,WR=0,WQ=0,WK=0,BP=0,BN=0,BB=0,BR=0,BQ=0,BK=0;

  castleBools castleInfo; // This isn't really part of the bitboards, but makes the game object cleaner

private:
  //U64 arrayToBitboard(const char array[64], char character);


public:
  void bitboardToArray(const U64 bitboard, char array[64], char character);
  U64 arrayToBitboard(const char array[64], char character);
  void initStandardBoard(); // Fills the bitboards with the setup for standard chess
  void convertToStandardArray(char array[64]); // Converts the bitboards into a char array, using a "standard" characters for certain pieces
  void printStandardArrayBoard(); // Prints the board to cout, using the "standard" characters provided by convertToStandardArray()
};
