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

#include <RGBLED.h>
#include <RGBColor.h>

/*
 * red pin 9; green pin 10, blue pin 11
 * hold 0ms, wait 2ms, common anode
*/
RGBLED rgbLED = RGBLED(9, 10, 11, 0, 2, 1);

RGBColor black = RGBColor(0, 0, 0);
RGBColor white = RGBColor(100, 100, 100);
RGBColor red   = RGBColor(100, 0, 0);
RGBColor green = RGBColor(0, 100, 0);
RGBColor blue  = RGBColor(0, 0, 100);

void setup() {
  rgbLED.setIntensity(100);
  rgbLED.setColor(black);
}

void loop() {
  rgbLED.crossfade(white);
  
  rgbLED.crossfade(red);
  
  rgbLED.crossfade(green);
  
  rgbLED.setCrossfadeHold(1);
  rgbLED.setCrossfadeWait(0);
  rgbLED.crossfade(blue);
  rgbLED.setCrossfadeHold(0);
  rgbLED.setCrossfadeWait(2);
}

