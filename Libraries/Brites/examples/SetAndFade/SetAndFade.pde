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

#include <Brites.h>
#include <BriteColor.h>

/*
 * 1 megabrite
 * hold 0ms, wait 2ms, common anode
 */
Brites brites = Brites(1, 0, 2);

BriteColor black = BriteColor(0, 0, 0);
BriteColor white = BriteColor(100, 100, 100);
BriteColor red   = BriteColor(100, 0, 0);
BriteColor green = BriteColor(0, 100, 0);
BriteColor blue  = BriteColor(0, 0, 100);

void setup() {
  brites.setColor(black);
}

void loop() {
  brites.setColor(white);
  brites.crossfade(red);

  brites.crossfade(green);

  brites.crossfade(blue);

  brites.crossfade(white);

  brites.crossfade(black);

}


