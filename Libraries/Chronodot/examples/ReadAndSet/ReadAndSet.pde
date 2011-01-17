/*
 * Copyright, 2011 Radek Wierzbicki
 *  
 * This file is part of ChronodotLib.
 *
 * ChronodotLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ChronodotLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with ChronodotLib. If not, see <http://www.gnu.org/licenses/>.
*/


#include <Wire.h>
#include <Chronodot.h>


// create global chronodot instance
Chronodot chronodot = Chronodot();


void setup() {
  Serial.begin(9600);
}


void loop() {
  // try to set the date and time if anything
  // shows up on the serial line
  if(Serial.available()) {
    timeDateElements tE;
    processSyncMessage(tE);
    if (tE.month > 0) {
      chronodot.setTimeDate(tE);
    }
  }
  // read and send the time over serial
  sendTime();
  delay(1000);
}


void sendTime() {
  chronodot.readTimeDate();
  
  Serial.print(chronodot.timeDate.year);
  Serial.print("-");
  printPadded(chronodot.timeDate.month);
  Serial.print("-");
  printPadded(chronodot.timeDate.day);
  Serial.print(" ");
  printPadded(chronodot.timeDate.hours);
  Serial.print(":");
  printPadded(chronodot.timeDate.minutes);
  Serial.print(":");
  printPadded(chronodot.timeDate.seconds);
  Serial.println("");
}


void printPadded(int num) {
  if(num < 10)
    Serial.print('0');
  Serial.print(num);
}


// get the date and time from serial in the following format
// TYYYY-MM-DD HH:MM:SS
void processSyncMessage(timeDateElements &tE) {
  tE.seconds = 0;
  tE.minutes = 0;
  tE.hours   = 0;
  tE.weekDay = 0;
  tE.day     = 0;
  tE.month   = 0;
  tE.year    = 0;
  
  while (Serial.available() >= 15) {
    char c = Serial.read();
    if (c == 'T') {
      int d1000, d100, d10, d01;
      
      d1000 = Serial.read() - '0';
      d100 = Serial.read() - '0';
      d10 = Serial.read() - '0';
      d01 = Serial.read() - '0';
      tE.year = (d1000 * 1000) + (d100 * 100) + (d10 * 10) + d01;
      
      d10 = Serial.read() - '0';
      d01 = Serial.read() - '0';
      tE.month = (d10 * 10) + d01;
      
      d10 = Serial.read() - '0';
      d01 = Serial.read() - '0';
      tE.day = (d10 * 10) + d01;
      
      d10 = Serial.read() - '0';
      d01 = Serial.read() - '0';
      tE.hours = (d10 * 10) + d01;
      
      d10 = Serial.read() - '0';
      d01 = Serial.read() - '0';
      tE.minutes = (d10 * 10) + d01;
      
      d10 = Serial.read() - '0';
      d01 = Serial.read() - '0';
      tE.seconds = (d10 * 10) + d01;
    }
  }
}

