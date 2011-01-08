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

#include "WProgram.h"
#include "RGBColor.h"

RGBColor::RGBColor(int red, int green, int blue) {
    _red = constrain(red, 0, 100);
    _green = constrain(green, 0, 100);
    _blue = constrain(blue, 0, 100);
}


int RGBColor::getRed() {
    return _red;
}

int RGBColor::getGreen() {
    return _green;
}

int RGBColor::getBlue() {
    return _blue;
}

void RGBColor::setRed(int red) {
    _red = constrain(red, 0, 100);
}

void RGBColor::setGreen(int green) {
    _green = constrain(green, 0, 100);
}

void RGBColor::setBlue(int blue) {
    _blue = constrain(blue, 0, 100);
}

