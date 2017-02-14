/*
 *  Manage strings by using a generic, dynamic queue data structure.
 *
 *  Copyright (C) 2010  Efstathios Chatzikyriakidis (contact@efxa.org)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

// include queue library header.
#include <QueueList.h>

enum{ FOREHAND, BACKHAND};

// declare string messages.
typedef struct{
  int styrka;
  bool typ;
  int tid;
}slag;

// create a queue of strings messages.
QueueList <struct> queue;

// startup point entry (runs once).
void
setup () {
  // start serial communication.
  Serial.begin (9600);

  // set the printer of the queue.
  queue.setPrinter (Serial);

  slag slag1;
  slag1.styrka = 500;
  slag1.typ = FOREHAND;
  slag1.tid = 897;

  // push all the string messages to the queue.
  queue.push (slag1);

  // pop all the string messages from the queue.
  while (!queue.isEmpty ())
    //Serial.println (queue.pop ());
}

// loop the main sketch.
void
loop () {
  // nothing here.
}
