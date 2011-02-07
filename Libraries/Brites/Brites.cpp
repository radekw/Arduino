/*
 * Copyright, 2011 Radek Wierzbicki
 *  
 * This file is part of Brites.
 *
 * Brites is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Brites is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with Brites.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "WProgram.h"
#include "Brites.h"

/* 
 * Library by Radek Wierzbicki
 * Based on ColorCrossfader sketch by Clay Shirky <clay.shirky@nyu.edu>
 * http://www.arduino.cc/en/Tutorial/ColorCrossfader
 * 
 * The library works with common anode and common cathode LED's
 * It will use delays from TimeAlarms library if used
 * http://www.arduino.cc/playground/Code/Time
 * 
 * 
 * The program works like this:
 * Imagine a crossfade that moves the red LED from 0-10, 
 *   the green from 0-5, and the blue from 10 to 7, in
 *   ten steps.
 *   We'd want to count the 10 steps and increase or 
 *   decrease color values in evenly stepped increments.
 *   Imagine a + indicates raising a value by 1, and a -
 *   equals lowering it. Our 10 step fade would look like:
 * 
 *   1 2 3 4 5 6 7 8 9 10
 * R + + + + + + + + + +
 * G   +   +   +   +   +
 * B     -     -     -
 * 
 * The red rises from 0 to 10 in ten steps, the green from 
 * 0-5 in 5 steps, and the blue falls from 10 to 7 in three steps.
 * 
 * In the real program, the color percentages are converted to 
 * 0-MAX_COLOR_VALUE values, and there are STEPS steps.
 * 
 * To figure out how big a step there should be between one up- or
 * down-tick of one of the LED values, we call _calculateStep(), 
 * which calculates the absolute gap between the start and end values, 
 * and then divides that gap by MAX_COLOR_VALUE to determine the size of the step  
 * between adjustments in the value.
*/



/*
 * Constructor
 * numOfBrites - number of Brites (max 10)
 * hold - internal crossfade delay in miliseconds; increase for slower fades
 * wait - hold the color after the fade for X miliseconds
 * commonAnode - set to 1 when using common anode LED, 0 for common cathode
*/
Brites::Brites(int numOfBrites, int hold, int wait) {
  _numOfBrites = constrain(numOfBrites, 1, MAX_BRITES);
  _hold = hold;
  _wait = wait;

  for (int b = 0; b < MAX_BRITES; b++) {
    for (int c = 0; c < 3; c++) {
      _ledChannels[b][c] = 0;
    }
  }

  _redValue = 0;
  _greenValue = 0;
  _blueValue = 0;

  _prevRed = 0;
  _prevGreen = 0;
  _prevBlue = 0;

  pinMode(DATA_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  SPCR = (1 << SPE) | (1 << MSTR) | (0 << SPR1) | (0 << SPR0);
  digitalWrite(LATCH_PIN, LOW);
  digitalWrite(ENABLE_PIN, LOW);
}



/* crossFade() converts the percentage colors to a 
 *  0-MAX_COLOR_VALUE range, then loops STEPS times, checking to see if  
 *  the value needs to be updated each time, then writing
 *  the color values to the correct pins.
*/
void Brites::crossfade(BriteColor &color) {
  // map color to values from 0 to MAX_COLOR_VALUE
  int R = _mapColor(color.getRed());
  int G = _mapColor(color.getGreen());
  int B = _mapColor(color.getBlue());
  
  int stepR = _calculateStep(_prevRed, R);
  int stepG = _calculateStep(_prevGreen, G); 
  int stepB = _calculateStep(_prevBlue, B);

  for (int i = 0; i <= STEPS; i++) {
    _redValue = _calculateVal(stepR, _redValue, i);
    _greenValue = _calculateVal(stepG, _greenValue, i);
    _blueValue = _calculateVal(stepB, _blueValue, i);

    // Write current values to color array
    for (int b = 0; b < _numOfBrites; b++) {
        _ledChannels[b][0] = _redValue;
        _ledChannels[b][1] = _greenValue;
        _ledChannels[b][2] = _blueValue;
    }
    writeLEDArray();

    // Pause for 'wait' milliseconds before resuming the loop
#ifdef TimeAlarms_h    
    Alarm.delay(_wait);
#else
    delay(_wait);
#endif
  }
  // Update current values for next loop
  _prevRed = _redValue; 
  _prevGreen = _greenValue; 
  _prevBlue = _blueValue;
  // Pause for optional 'wait' milliseconds before resuming the loop
#ifdef TimeAlarms_h    
    Alarm.delay(_hold);
#else
    delay(_hold);
#endif
}



/*
 * setColor() sets a color without crossfade
*/
void Brites::setColor(BriteColor &color) {
  _redValue = _mapColor(color.getRed());
  _greenValue = _mapColor(color.getGreen());
  _blueValue = _mapColor(color.getBlue());

  // Write current values to color array
  for (int b = 0; b < _numOfBrites; b++) {
    _ledChannels[b][0] = _redValue;
    _ledChannels[b][1] = _greenValue;
    _ledChannels[b][2] = _blueValue;
  }
  writeLEDArray();

  _prevRed = _redValue; 
  _prevGreen = _greenValue; 
  _prevBlue = _blueValue;
}


/*
 * set crossfade wait in miliseconds
 * accepts an integer
*/
void Brites::setCrossfadeWait(int wait) {
    _wait = wait;
}



/*
 * set crossfade hold in miliseconds
 * accepts and integer
*/
void Brites::setCrossfadeHold(int hold) {
    _hold = hold;
}



/*
 * 
*/
int Brites::_calculateStep(int prevValue, int endValue) {
  int step = endValue - prevValue; // What's the overall gap?
  if (step) {                      // If its non-zero, 
    step = (STEPS) / step;          // divide into STEPS
  } 
  return step;
}



/* When the loop value, i,
 *  reaches the step size appropriate for one of the
 *  colors, it increases or decreases the value of that color by 1. 
 *  (R, G, and B are each calculated separately.)
*/
int Brites::_calculateVal(int step, int val, int i) {
  if ((step) && i % step == 0) { // If step is non-zero and its time to change a value,
    if (step > 0) {              // increment the value if step is positive...
      val += 1;           
    } else if (step < 0) {       // ...or decrement it if step is negative
      val -= 1;
    } 
  }
  // Defensive driving: make sure val stays in the range 0-MAX_COLOR_VALUE
  val = constrain(val, 0, MAX_COLOR_VALUE);
  return val;
}



/* _mapColor() maps color described by value from 0 to 100
 * to value from 0 to MAX_COLOR_VALUE according to colorMap
*/
int Brites::_mapColor(int color) {
  return colorMap[color];
  //return map(color, 0, MAX_COLOR, 0, MAX_COLOR_VALUE);
}



void Brites::sendPacket() {
  if (_commandMode == B01) {
    _redCommand = 120;
    _greenCommand = 100;
    _blueCommand = 100;
  }
 
  SPDR = _commandMode << 6 | _blueCommand>>4;
  while(!(SPSR & (1<<SPIF)));
  SPDR = _blueCommand<<4 | _redCommand>>6;
  while(!(SPSR & (1<<SPIF)));
  SPDR = _redCommand << 2 | _greenCommand>>8;
  while(!(SPSR & (1<<SPIF)));
  SPDR = _greenCommand;
  while(!(SPSR & (1<<SPIF)));
}



void Brites::writeLEDArray(void) {
  _commandMode = B00; // Write to PWM control registers
  for (int h = 0; h < _numOfBrites; h++) {
    _redCommand = _ledChannels[h][0];
    _greenCommand = _ledChannels[h][1];
    _blueCommand = _ledChannels[h][2];
    sendPacket();
  }

  delayMicroseconds(15);
  digitalWrite(LATCH_PIN,HIGH); // latch data into registers
  delayMicroseconds(15);
  digitalWrite(LATCH_PIN,LOW);
 
  _commandMode = B01; // Write to current control registers
  for (int z = 0; z < _numOfBrites; z++) sendPacket();
  delayMicroseconds(15);
  digitalWrite(LATCH_PIN,HIGH); // latch data into registers
  delayMicroseconds(15);
  digitalWrite(LATCH_PIN,LOW);
}


