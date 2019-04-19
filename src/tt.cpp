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

#include <tt.h>

U64 randomValue(const U64 seed) {
  return XXH64(0x0, 0, seed);
}

void TT::init(const U64 seed) {
  U64 state = seed;
  state = randomValue(state);

  for(int i = 0; i < 64; i++)
  {
    state = randomValue(state);
    hashWP[i] = state;
  }
  for(int i = 0; i < 64; i++)
  {
    state = randomValue(state);
    hashWN[i] = state;
  }
  for(int i = 0; i < 64; i++)
  {
    state = randomValue(state);
    hashWB[i] = state;
  }
  for(int i = 0; i < 64; i++)
  {
    state = randomValue(state);
    hashWR[i] = state;
  }
  for(int i = 0; i < 64; i++)
  {
    state = randomValue(state);
    hashWQ[i] = state;
  }
  for(int i = 0; i < 64; i++)
  {
    state = randomValue(state);
    hashWK[i] = state;
  }
  for(int i = 0; i < 64; i++)
  {
    state = randomValue(state);
    hashBP[i] = state;
  }
  for(int i = 0; i < 64; i++)
  {
    state = randomValue(state);
    hashBN[i] = state;
  }
  for(int i = 0; i < 64; i++)
  {
    state = randomValue(state);
    hashBB[i] = state;
  }
  for(int i = 0; i < 64; i++)
  {
    state = randomValue(state);
    hashBR[i] = state;
  }
  for(int i = 0; i < 64; i++)
  {
    state = randomValue(state);
    hashBQ[i] = state;
  }
  for(int i = 0; i < 64; i++)
  {
    state = randomValue(state);
    hashBK[i] = state;
  }
  state = randomValue(state);
  hashSide = state;
  state = randomValue(state);
  hashWhiteA = state;
  state = randomValue(state);
  hashWhiteH = state;
  state = randomValue(state);
  hashBlackA = state;
  state = randomValue(state);
  hashBlackH = state;
  for(int i = 0; i < 8; i++)
  {
    state = randomValue(state);
    hashEnPassant[i] = state;
  }
}

/*U64 TT::getHash() {
  U64 state = 0ULL;

  for(int i = 0; i < 64; i++)
  {
    state ^= hashWP[i];
  }
  for(int i = 0; i < 64; i++)
  {
    state ^= hashWN[i];
  }
  for(int i = 0; i < 64; i++)
  {
    state ^= hashWB[i];
  }
  for(int i = 0; i < 64; i++)
  {
    state ^= hashWR[i];
  }
  for(int i = 0; i < 64; i++)
  {
    state ^= hashWQ[i];
  }
  for(int i = 0; i < 64; i++)
  {
    state ^= hashWK[i];
  }
  for(int i = 0; i < 64; i++)
  {
    state ^= hashBP[i];
  }
  for(int i = 0; i < 64; i++)
  {
    state ^= hashBN[i];
  }
  for(int i = 0; i < 64; i++)
  {
    state ^= hashBB[i];
  }
  for(int i = 0; i < 64; i++)
  {
    state ^= hashBR[i];
  }
  for(int i = 0; i < 64; i++)
  {
    state ^= hashBQ[i];
  }
  for(int i = 0; i < 64; i++)
  {
    state ^= hashBK[i];
  }
  state ^= hashSide;
  state ^= hashWhiteA;
  state ^= hashWhiteH;
  state ^= hashBlackA;
  state ^= hashWhiteH;
  for(int i = 0; i < 64; i++)
  {
    state ^= hashEnPassant[i];
  }
  return state;
}*/
