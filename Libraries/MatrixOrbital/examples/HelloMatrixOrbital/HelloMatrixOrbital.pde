/*
 * Copyright, 2011 Radek Wierzbicki
 *  
 * This file is part of MatrixOrbital.
 *
 * MatrixOrbital is free ware: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MatrixOrbital is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with MatrixOrbital.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <MatrixOrbital.h>


MatrixOrbital lcd = MatrixOrbital(2, 20);

void setup() {
  Serial.begin(19200);
  lcd.turnBlockCursorOff();
  lcd.turnUnderlineCursorOff();
  lcd.backlightOff();
  lcd.clear();
  delay(500);
  lcd.backlightOn(0);
}


void loop()
{
  lcd.clear();
  delay(500);
  
  lcd.print("Matrix Orbital");
  lcd.newLine();
  delay(500);
  lcd.print("Hello");
  lcd.newLine();
  delay(1000);
}


