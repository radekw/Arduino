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
#include "BriteColor.h"

BriteColor::BriteColor(int red, int green, int blue) {
    _red = constrain(red, 0, MAX_COLOR);
    _green = constrain(green, 0, MAX_COLOR);
    _blue = constrain(blue, 0, MAX_COLOR);
}


int BriteColor::getRed() {
    return _red;
}

int BriteColor::getGreen() {
    return _green;
}

int BriteColor::getBlue() {
    return _blue;
}

void BriteColor::setRed(int red) {
    _red = constrain(red, 0, MAX_COLOR);
}

void BriteColor::setGreen(int green) {
    _green = constrain(green, 0, MAX_COLOR);
}

void BriteColor::setBlue(int blue) {
    _blue = constrain(blue, 0, MAX_COLOR);
}

