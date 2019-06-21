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

#include <game.h>
#include <moves.h>
#include <iostream> // For debugging
#include <utilities.h> // Also for debugging

struct flexScore {
  double score = -1e99; // The current evaulation of this position
  int count; // The half turns left until a mate/draw
  bool draw = false; // Is the count for a draw? If not it is for a mate
  bool tethered = false; // Should we use the mate/draw counter? If not we would be floating (using score)
  bool favorable; // If tethered, would the result be a win for us?

  void operator=(const flexScore& b) {
    score = b.score;
    count = b.count;
    draw = b.draw;
    tethered = b.tethered;
    favorable = b.favorable;
  }

  flexScore operator-() {
    flexScore returner = *this;
    returner.favorable = !returner.favorable;
    returner.count--;
    return returner;
  }

/*
if(b.tethered) {
  if(b.draw) {

  } else {

  }
} else {

}
*/
  bool operator>(const flexScore& b) { // There's probably a more readable way to do this...
    if(this->tethered) {
      if(this->draw) {
        if(b.tethered) {
          if(b.draw) {
            return this->count > b.count;
          } else {
            return !b.favorable;
          }
        }
        return false;
      } else {
        if(b.tethered) {
          if(b.draw) {
            return this->favorable;
          } else {
            if(this->favorable) {
              if(b.favorable) {
                return this->count < b.count;
              }
              return true;
            } else {
              if(b.favorable) {
                return false;
              } else {
                return this->count > b.count;
              }
            }
          }
        } else {
          return this->favorable;
        }
      }
    } else {
      if(b.tethered) {
        if(b.draw) {
          return true;
        } else {
          return !b.favorable;
        }
      } else {
        return this->score > b.score;
      }
    }
    return false;
  }
};

void evaluate(game Game, move& bestMove); // Add mutexes if threaded
