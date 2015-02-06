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

#ifndef BriteColor_h
#define BriteColor_h

#include "Arduino.h"

#define MAX_COLOR 100

class BriteColor {
    public:
        BriteColor(int red, int green, int blue);
        int getRed();
        int getGreen();
        int getBlue();
        void setRed(int red);
        void setGreen(int green);
        void setBlue(int blue);
    private:
        int _red;
        int _green;
        int _blue;
};

#endif

