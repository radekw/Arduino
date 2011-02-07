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

#include "WProgram.h"
#include "BriteColor.h"

#define CLOCK_PIN 13  // CI
#define ENABLE_PIN 10 // EI
#define LATCH_PIN 9   // LI
#define DATA_PIN 11   // DI

#define MAX_BRITES 10
#define MAX_COLOR_VALUE 1023
#define STEPS 1023

const int colorMap[101] = {
0, 5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100, 105, 110, 115, 120, 125, 130, 135, 140, 145, 150, 155, 160, 165, 170, 175, 180, 185, 190, 195, 200, 205, 210, 215, 220, 225, 230, 235, 240, 245, 250, 255, 260, 265, 270, 275, 280, 285, 290, 295, 300, 305, 310, 315, 320, 325, 330, 335, 340, 345, 350, 355, 360, 365, 370, 375, 380, 385, 390, 395, 400, 405, 410, 434, 468, 503, 537, 572, 607, 641, 676, 711, 745, 780, 815, 849, 884, 919, 953, 988, 1023
};

class Brites {
    public:
        Brites(int numOfBrites, int hold, int wait);
        void setup();
        void crossfade(BriteColor &color);
        void setColor(BriteColor &color);
        void setCrossfadeWait(int wait);
        void setCrossfadeHold(int hold);
    private:
        int _ledChannels[MAX_BRITES][3];

        int _commandMode;
        int _redCommand;
        int _greenCommand;
        int _blueCommand;
        
        int _numOfBrites;
        int _hold;
        int _wait;

        int _redValue;
        int _greenValue;
        int _blueValue;

        int _prevRed;
        int _prevGreen;
        int _prevBlue;

        int _calculateStep(int prevValue, int endValue);
        int _calculateVal(int step, int val, int i);
        int _mapColor(int color);

        void writeLEDArray(void);
        void sendPacket(void);
};

#endif

