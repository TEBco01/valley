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

void boardStack::push(bitboards boards) {
  boardNode* node = new boardNode();
  node->boards = boards;
  node->last = tail;
  tail = node;
}

bitboards boardStack::pop() {
 boardNode* newTail = tail->last;
 bitboards boards = tail->boards;
 delete tail;
 tail = newTail;
 return boards;
}

void moveStack::push(move a){
	movePancake *pancake = new movePancake;
	pancake->data = a;
	pancake->last = tail;
	tail = pancake;
};

move moveStack::pop(){
	movePancake *newtail = tail->last;
	move moves = tail->data;
	delete tail;
	tail = newtail;
	return moves;
};

void moveStack::emptyPop(){
	movePancake *newtail = tail->last;
	delete tail;
	tail = newtail;
};

move moveStack::peek(){
  if(tail != NULL)
	 return tail->data;
  else
  {
    move emptyMove;
    return emptyMove;
  }
};
