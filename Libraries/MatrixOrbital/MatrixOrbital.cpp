/*
 * Copyright, 2011 Radek Wierzbicki
 *  
 * This file is part of MatrixOrbital.
 *
 * MatrixOrbital is free software: you can redistribute it and/or modify
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

#include "MatrixOrbital.h"
#include "MatrixOrbitalConstants.h"

// http://www.arduino.cc/playground/Learning/SerialLCD
// http://laserpup.com/?p=114


MatrixOrbital::MatrixOrbital(int rows, int columns) {
    Serial.begin(19200);
    _rows = rows;
    _columns = columns;
}



void MatrixOrbital::write(uint8_t b)
{
	Serial.print(b);
}



void MatrixOrbital::turnAutoScrollOn() {
    Serial.print(LCD_CMD, BYTE);
    Serial.print(LCD_AUTOSCROLLON, BYTE);
}



void MatrixOrbital::turnAutoScrollOff() {
    Serial.print(LCD_CMD, BYTE);
    Serial.print(LCD_AUTOSCROLLOFF, BYTE);
}



void MatrixOrbital::turnAutoLineWrapOn() {
    Serial.print(LCD_CMD, BYTE);
    Serial.print(LCD_AUTOWRAPON, BYTE);
}



void MatrixOrbital::turnAutoLineWrapOff() {
    Serial.print(LCD_CMD, BYTE);
    Serial.print(LCD_AUTOWRAPOFF, BYTE);
}



void MatrixOrbital::turnBlockCursorOn() {
    Serial.print(LCD_CMD, BYTE);
    Serial.print(LCD_BLOCKON, BYTE);
}



void MatrixOrbital::turnBlockCursorOff() {
    Serial.print(LCD_CMD, BYTE);
    Serial.print(LCD_BLOCKOFF, BYTE);
}



void MatrixOrbital::turnUnderlineCursorOn() {
    Serial.print(LCD_CMD, BYTE);
    Serial.print(LCD_UNDERLINEON, BYTE);
}



void MatrixOrbital::turnUnderlineCursorOff() {
    Serial.print(LCD_CMD, BYTE);
    Serial.print(LCD_UNDERLINEOFF, BYTE);
}



void MatrixOrbital::clear() {
    Serial.print(LCD_CLEAR, BYTE);
}



void MatrixOrbital::newLine() { 
    Serial.print(LCD_NEWLINE, BYTE);
}



void MatrixOrbital::cursorHome() {
    Serial.print(LCD_CMD, BYTE);
    Serial.print(LCD_CURSORHOME, BYTE);
}



void MatrixOrbital::cursorSet(int x, int y) {
    Serial.print(LCD_CMD, BYTE);
    Serial.print(LCD_CURSORSET, BYTE);
    Serial.print(x);
    Serial.print(y);
} 



void MatrixOrbital::backspace() {
    Serial.print(LCD_BACKSPACE, BYTE);
}



void MatrixOrbital::cursorLeft() {
    Serial.print(LCD_CMD, BYTE);
    Serial.print(LCD_CURSORLEFT, BYTE);
}



void MatrixOrbital::cursorRight() {
    Serial.print(LCD_CMD, BYTE);
    Serial.print(LCD_CURSORRIGHT, BYTE);
}



void MatrixOrbital::setContrast(int contrast) {
    Serial.print(LCD_CMD, BYTE);
    Serial.print(LCD_CONTRAST, BYTE);
    Serial.print(contrast);
}



void MatrixOrbital::backlightOn(int minutes) {
    Serial.print(LCD_CMD, BYTE);
    Serial.print(LCD_BACKLIGHTON, BYTE);
    // use 0 minutes to turn the backlight on indefinitely
    Serial.print(minutes);
}



void MatrixOrbital::backlightOff() {
    Serial.print(LCD_CMD, BYTE);
    Serial.print(LCD_BACKLIGHTOFF, BYTE);
}



