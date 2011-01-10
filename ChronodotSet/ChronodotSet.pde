/*
 * Copyright, 2011 Radek Wierzbicki
 *  
 * This file is part of ChronodotSet.
 *
 * ChronodotSet is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ChronodotSet is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with ChronodotSet.  If not, see <http://www.gnu.org/licenses/>.
*/


#include <Time.h>
#include <Wire.h>
#include <DS1307RTC.h>


void setup() {
  Serial.begin(9600);
  Wire.begin();
}


void loop() {
  if(Serial.available()) {
     time_t t = processSyncMessage();
     if(t > 0) {
        RTC.set(t);
     }
  }
  sendTime();
  delay(1000);
}


void sendTime() {
  tmElements_t tm;
  RTC.read(tm);
  
  Serial.print((int)tm.Year + 1970);
  Serial.print("-");
  padZero((int)tm.Month);
  Serial.print("-");
  padZero((int)tm.Day);
  Serial.print(" ");
  padZero((int)tm.Hour);
  Serial.print(":");
  padZero((int)tm.Minute);
  Serial.print(":");
  padZero((int)tm.Second);
  Serial.println("");
}


void padZero(int num) {
  if(num < 10)
    Serial.print('0');
  Serial.print(num);
}


/*  code to process time sync messages from the serial port   */
#define TIME_MSG_LEN  11   // time sync to PC is HEADER followed by unix time_t as ten ascii digits
#define TIME_HEADER  'T'   // Header tag for serial time sync message


time_t processSyncMessage() {
  // return the time if a valid sync message is received on the serial port.
  while (Serial.available() >=  TIME_MSG_LEN) {  // time message consists of a header and ten ascii digits
    char c = Serial.read();
    Serial.print(c);
    if (c == TIME_HEADER) {
      time_t pctime = 0;
      for(int i = 0; i < TIME_MSG_LEN - 1; i++) {
        c = Serial.read();
        if (c >= '0' && c <= '9') {
          pctime = (10 * pctime) + (c - '0') ; // convert digits to a number
        }
      }
      return pctime;
    }
  }
  return 0;
}

