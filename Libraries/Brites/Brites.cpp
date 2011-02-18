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



/*
 * cross fade between current color and the new one
*/
void Brites::crossfade(BriteColor &color) {
  
  float stepR = _calculateStep(_prevRed, color.getRed());
  float stepG = _calculateStep(_prevGreen, color.getGreen()); 
  float stepB = _calculateStep(_prevBlue, color.getBlue());

  for (int i = 0; i <= STEPS; i++) {
    _redValue = _calculateVal(stepR, _redValue);
    _greenValue = _calculateVal(stepG, _greenValue);
    _blueValue = _calculateVal(stepB, _blueValue);


    // Write current values to color array
    for (int b = 0; b < _numOfBrites; b++) {
        // map color to values from 0 to MAX_COLOR_VALUE
        _ledChannels[b][0] = colorMap[(int)_redValue];
        _ledChannels[b][1] = colorMap[(int)_greenValue];
        _ledChannels[b][2] = colorMap[(int)_blueValue];
    }
    writeLEDArray();

    // Pause for 'wait' milliseconds before resuming the loop
    delay(_wait);
  }
  // update current values for next loop
  _prevRed = _redValue; 
  _prevGreen = _greenValue; 
  _prevBlue = _blueValue;
  // Pause for optional 'wait' milliseconds before resuming the loop
  delay(_hold);
}



/*
 * setColor() sets a color without crossfade
*/
void Brites::setColor(BriteColor &color) {
  // Write current values to color array
  for (int b = 0; b < _numOfBrites; b++) {
    _ledChannels[b][0] = colorMap[color.getRed()];
    _ledChannels[b][1] = colorMap[color.getGreen()];
    _ledChannels[b][2] = colorMap[color.getBlue()];
  }
  writeLEDArray();

  _prevRed = _redValue; 
  _prevGreen = _greenValue; 
  _prevBlue = _blueValue;
}


/*
 * set crossfade wait in miliseconds
*/
void Brites::setCrossfadeWait(int wait) {
    _wait = wait;
}



/*
 * set crossfade hold in miliseconds
*/
void Brites::setCrossfadeHold(int hold) {
    _hold = hold;
}



/*
 * get crossfade wait in miliseconds
*/
int Brites::getCrossfadeWait(void) {
    return _wait;
}



/*
 * get crossfade hold in miliseconds
*/
int Brites::getCrossfadeHold(void) {
    return _hold;
}



/*
 * calculate step between color values
*/
float Brites::_calculateStep(float prevValue, float endValue) {
  return (endValue - prevValue) / STEPS;
}



/*
 * add step to the color value and prevent overflow
*/
float Brites::_calculateVal(float step, float val) {
  val = val + step;
  if (val < 0)
      val = 0;
  else if (val > 100)
      val = 100;
  return val;
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


