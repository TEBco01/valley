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

#include <moves.h>

void linkedMoveList::add(move value){
	moveNode *temp = new moveNode;
	temp->data = value;
	temp->next = NULL;
	if(head == NULL)
	{
		head = temp;
		tail = temp;
		temp = NULL;
	}
	else
	{
		tail->next = temp;
		tail = temp;
	}
};

void linkedMoveList::create(byte start, byte end, byte special){
	move newMove;
	newMove.start = start;
	newMove.end = end;
	newMove.special = special;
	this->add(newMove);
};

void linkedMoveList::remove(int pos){
	if(pos == 0) {
		moveNode *temp = head;
		head = head->next;
		delete temp;
	} else {
		moveNode *current = head->next;
		moveNode *previous = head;
		for(int i = 1; i < pos; i++){
			previous = current;
			current = current->next;
		}
		previous->next = current->next;
		if(current == tail) tail = previous;
		delete current;
	}
};

void linkedMoveList::operator=(linkedMoveList b){
	this->head = b.head;
	this->tail = b.tail;
};

void linkedMoveList::deleteList(){
  moveNode *cur;
  moveNode *temp;
  cur = head;
	temp = head;
	while(temp != NULL){
    temp = cur->next;
		delete cur;
    cur = temp;
	}

  head = NULL;
  tail = NULL;
};

void linkedMoveList::operator+=(linkedMoveList b){
  if(this->head == NULL) {
    *this = b;
  } else {
    this->tail->next = b.head;
    if(b.head != NULL)
      this->tail = b.tail;
  }
};
