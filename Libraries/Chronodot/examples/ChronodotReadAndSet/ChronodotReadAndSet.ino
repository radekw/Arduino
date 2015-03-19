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

/*
 * Modifications by Chris "Sembazuru" Elliott, SembazuruCDE (at) GMail.com
 * 2014-09-21
 *
 * Modified from the ReadAndSet.pde example to set square wave output to 1Hz
 * and report it's state to find out if the rising or falling edge coincides
 * with the start of the second.
 *
 * Also on every transition line printout instructions for setting the clock.
 *
 * Also set day of week where 1=Sunday, 2=Monday, etc.
 *
 * Also added comments of my interpretation of what is happening
 */

#include <Wire.h>
#include <Chronodot.h>


// create global chronodot instance
Chronodot chronodot = Chronodot();

// remember last square wave state
const byte SQWpin = 2;
boolean lastSQW;

// Week Day Name table with padding
const char* weekDayName[] =
{
  "Sunday   ",
  "Monday   ",
  "Tuesday  ",
  "Wednesday",
  "Thursday ",
  "Friday   ",
  "Saturday "
};


void setup()
{
  Serial.begin(115200);
  chronodot.setSQW(1); // Set square wave pin output of 1Hz
  pinMode(SQWpin,INPUT_PULLUP);
  pinMode(LED_BUILTIN,OUTPUT);
  lastSQW = digitalRead(SQWpin);
}


void loop()
{
  // try to set the date and time if anything
  // shows up on the serial line
  if(Serial.available())
  {
    timeDateElements tE;
    processSyncMessage(tE); // won't actually process anything if the serial buffer doesn't have enough characters for a full message
    if (tE.month > 0) // probably could be any of the timeDate elements that has 0 as an unacceptable value, i.e. month, day, or weekDay.
    {
      chronodot.setTimeDate(tE);
    }
  }
  digitalWrite(LED_BUILTIN,digitalRead(SQWpin));
  // read and send the time over serial
  lastSQW = sendTime(lastSQW);
  delay(100);
}


boolean sendTime(boolean last)
{
  boolean current = digitalRead(SQWpin);
  boolean transition = false;
  chronodot.readTimeDate();

  if (last != current)
  {
    transition = true;
    Serial.print(F("--   "));
  }
  else
  {
    if (current)
    {
      Serial.print(F(" | 1 "));
    }
    else
    {
      Serial.print(F("|  0 "));
    }
  }

  Serial.print(chronodot.timeDate.year);
  Serial.print('-');
  printPadded(chronodot.timeDate.month);
  Serial.print('-');
  printPadded(chronodot.timeDate.day);
  Serial.print(' ');
  Serial.print(weekDayName[chronodot.timeDate.weekDay - 1]);
  Serial.print(' ');
  printPadded(chronodot.timeDate.hours);
  Serial.print(':');
  printPadded(chronodot.timeDate.minutes);
  Serial.print(':');
  printPadded(chronodot.timeDate.seconds);

  if (transition)
  {
    Serial.print(F(" Enter new date and time as 'TYYYYMMDDWhhmmss' where W=1 is Sunday, W=2 is Monday, etc."));
  }

  Serial.println();
  
  return current;
}


void printPadded(int num)
{
  if(num < 10)
  {
    Serial.print('0');
  }
  Serial.print(num);
}


// get the date and time from serial in the following format
// TYYYYMMDDWhhmmss
// where W is the day of the week with 1=Sunday, 2=Monday, etc.
void processSyncMessage(timeDateElements &tE)
{
  tE.seconds = 0;
  tE.minutes = 0;
  tE.hours   = 0;
  tE.weekDay = 0;
  tE.day     = 0;
  tE.month   = 0;
  tE.year    = 0;

  while (Serial.available() >= 16)
  {
    char c = Serial.read();
    if (c == 'T')
    {
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

      tE.weekDay = Serial.read() -'0';

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

