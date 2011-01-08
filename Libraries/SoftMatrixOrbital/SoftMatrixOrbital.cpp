/*
 * Copyright, 2011 Radek Wierzbicki
 *  
 * This file is part of SoftMatrixOrbital.
 *
 * SoftMatrixOrbital is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * SoftMatrixOrbital is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with SoftMatrixOrbital.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "SoftMatrixOrbital.h"
#include "MatrixOrbitalConstants.h"

// http://www.arduino.cc/playground/Learning/SerialLCD
// http://laserpup.com/?p=114


SoftMatrixOrbital::SoftMatrixOrbital(uint8_t rxPin, uint8_t txPin, int rows, int columns) : _serial(rxPin, txPin) {
    _serial.begin(19200);
    _rows = rows;
    _columns = columns;
}



void SoftMatrixOrbital::write(uint8_t b)
{
	_serial.print(b);
}



void SoftMatrixOrbital::turnAutoScrollOn() {
    _serial.print(LCD_CMD, BYTE);
    _serial.print(LCD_AUTOSCROLLON, BYTE);
}



void SoftMatrixOrbital::turnAutoScrollOff() {
    _serial.print(LCD_CMD, BYTE);
    _serial.print(LCD_AUTOSCROLLOFF, BYTE);
}



void SoftMatrixOrbital::turnAutoLineWrapOn() {
    _serial.print(LCD_CMD, BYTE);
    _serial.print(LCD_AUTOWRAPON, BYTE);
}



void SoftMatrixOrbital::turnAutoLineWrapOff() {
    _serial.print(LCD_CMD, BYTE);
    _serial.print(LCD_AUTOWRAPOFF, BYTE);
}



void SoftMatrixOrbital::turnBlockCursorOn() {
    _serial.print(LCD_CMD, BYTE);
    _serial.print(LCD_BLOCKON, BYTE);
}



void SoftMatrixOrbital::turnBlockCursorOff() {
    _serial.print(LCD_CMD, BYTE);
    _serial.print(LCD_BLOCKOFF, BYTE);
}



void SoftMatrixOrbital::turnUnderlineCursorOn() {
    _serial.print(LCD_CMD, BYTE);
    _serial.print(LCD_UNDERLINEON, BYTE);
}



void SoftMatrixOrbital::turnUnderlineCursorOff() {
    _serial.print(LCD_CMD, BYTE);
    _serial.print(LCD_UNDERLINEOFF, BYTE);
}



void SoftMatrixOrbital::clear() {
    _serial.print(LCD_CLEAR, BYTE);
}



void SoftMatrixOrbital::newLine() { 
    _serial.print(LCD_NEWLINE, BYTE);
}



void SoftMatrixOrbital::cursorHome() {
    _serial.print(LCD_CMD, BYTE);
    _serial.print(LCD_CURSORHOME, BYTE);
}



void SoftMatrixOrbital::cursorSet(int x, int y) {
    _serial.print(LCD_CMD, BYTE);
    _serial.print(LCD_CURSORSET, BYTE);
    _serial.print(x);
    _serial.print(y);
} 



void SoftMatrixOrbital::backspace() {
    _serial.print(LCD_BACKSPACE, BYTE);
}



void SoftMatrixOrbital::cursorLeft() {
    _serial.print(LCD_CMD, BYTE);
    _serial.print(LCD_CURSORLEFT, BYTE);
}



void SoftMatrixOrbital::cursorRight() {
    _serial.print(LCD_CMD, BYTE);
    _serial.print(LCD_CURSORRIGHT, BYTE);
}



void SoftMatrixOrbital::setContrast(int contrast) {
    _serial.print(LCD_CMD, BYTE);
    _serial.print(LCD_CONTRAST, BYTE);
    _serial.print(contrast);
}



void SoftMatrixOrbital::backlightOn(int minutes) {
    _serial.print(LCD_CMD, BYTE);
    _serial.print(LCD_BACKLIGHTON, BYTE);
    // use 0 minutes to turn the backlight on indefinitely
    _serial.print(minutes);
}



void SoftMatrixOrbital::backlightOff() {
    _serial.print(LCD_CMD, BYTE);
    _serial.print(LCD_BACKLIGHTOFF, BYTE);
}



