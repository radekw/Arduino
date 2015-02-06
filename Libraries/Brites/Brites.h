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

#ifndef Brites_h
#define Brites_h

#include "Arduino.h"
#include "BriteColor.h"

#define CLOCK_PIN 13  // CI
#define ENABLE_PIN 10 // EI
#define LATCH_PIN 9   // LI
#define DATA_PIN 11   // DI

#define MAX_BRITES 10
#define MAX_COLOR_VALUE 1023
#define STEPS 100

// making brites more linear
const int colorMap[101] = {
0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 
20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 44, 48, 52, 56, 60, 64, 68, 72, 76, 80, 
85, 90, 95, 100, 105, 110, 115, 120, 125, 130, 138, 146, 154, 162, 170, 178, 186, 194, 202, 210, 
220, 230, 240, 250, 260, 270, 280, 290, 300, 310, 330, 350, 370, 390, 410, 430, 450, 470, 490, 510, 
530, 550, 570, 590, 610, 630, 650, 670, 690, 710, 740, 770, 800, 830, 860, 890, 920, 950, 980, 1023
};

class Brites {
    public:
        Brites(int numOfBrites, int hold, int wait);
        void setup();
        void crossfade(BriteColor &color);
        void setColor(BriteColor &color);
        void setCrossfadeWait(int wait);
        void setCrossfadeHold(int hold);
        int getCrossfadeWait(void);
        int getCrossfadeHold(void);
    private:
        int _ledChannels[MAX_BRITES][3];

        int _commandMode;
        int _redCommand;
        int _greenCommand;
        int _blueCommand;
        
        int _numOfBrites;
        unsigned int _hold;
        unsigned int _wait;

        float _redValue;
        float _greenValue;
        float _blueValue;

        float _prevRed;
        float _prevGreen;
        float _prevBlue;

        float _calculateStep(float prevValue, float endValue);
        float _calculateVal(float step, float val);

        void writeLEDArray(void);
        void sendPacket(void);
};

#endif

