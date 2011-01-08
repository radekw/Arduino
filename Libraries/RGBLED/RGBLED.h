/*
 * Copyright, 2011 Radek Wierzbicki
 *  
 * This file is part of RGBLED.
 *
 * RGBLED is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RGBLED is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with RGBLED.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef RGBLED_h
#define RGBLED_h

#include "WProgram.h"
#include "RGBColor.h"

class RGBLED {
    public:
        RGBLED(int redPin, int greenPin, int bluePin, 
               int hold, int wait, int commonAnode);
        void setup();
        void crossfade(RGBColor &color);
        void setColor(RGBColor &color);
        void setIntensity(int intensity);
        void setCrossfadeWait(int wait);
        void setCrossfadeHold(int hold);
    private:
        int _commonAnode;

        int _hold;
        int _wait;

        float _intensity;

        int _redPin;
        int _greenPin;
        int _bluePin;

        int _redValue;
        int _greenValue;
        int _blueValue;

        int _prevRed;
        int _prevGreen;
        int _prevBlue;

        int _calculateStep(int prevValue, int endValue);
        int _calculateVal(int step, int val, int i);
        int _mapColor(int color);
};

#endif

