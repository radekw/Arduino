/*
 * Copyright, 2011 Radek Wierzbicki
 *  
 * This file is part of ChronodotLib.
 *
 * ChronodotLib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * ChronodotLib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with ChronodotLib. If not, see <http://www.gnu.org/licenses/>.
*/

#include "Wire.h"
#include "Chronodot.h"

#define DS3231_CTRL_ID 0x68 


uint8_t dec2bcd(uint8_t num) {
    return ((num / 10) << 4) + (num % 10);
}


uint8_t bcd2dec(uint8_t num) {
    return (((num >> 4) & 0b00001111) * 10) + (num & 0b00001111);
}


Chronodot::Chronodot() {
    Wire.begin();

    timeDateBCD.seconds = 0;
    timeDateBCD.minutes = 0;
    timeDateBCD.hours   = 0;
    timeDateBCD.weekDay = 0;
    timeDateBCD.day     = 0;
    timeDateBCD.month   = 0;
    timeDateBCD.year    = 0;

    timeDate.seconds = 0;
    timeDate.minutes = 0;
    timeDate.hours   = 0;
    timeDate.weekDay = 0;
    timeDate.day     = 0;
    timeDate.month   = 0;
    timeDate.year    = 0;

    temperature = 0;
}


void Chronodot::readTimeDate() {
    Wire.beginTransmission(DS3231_CTRL_ID);
    Wire.write((uint8_t)0x00);
    Wire.endTransmission();

    Wire.requestFrom(DS3231_CTRL_ID, 7);
    timeDateBCD.seconds = Wire.read() & 0b01111111; // ignore bit 7
    timeDateBCD.minutes = Wire.read();
    timeDateBCD.hours   = Wire.read() & 0b00111111; // ignore bit 6 and 7
    timeDateBCD.weekDay = Wire.read();
    timeDateBCD.day     = Wire.read();
    timeDateBCD.month   = Wire.read() & 0b01111111; // ignore bit 7

    timeDate.seconds = bcd2dec(timeDateBCD.seconds);
    timeDate.minutes = bcd2dec(timeDateBCD.minutes);
    timeDate.hours   = bcd2dec(timeDateBCD.hours);
    timeDate.weekDay = bcd2dec(timeDateBCD.weekDay);
    timeDate.day     = bcd2dec(timeDateBCD.day);
    timeDate.month   = bcd2dec(timeDateBCD.month);

    // some special handling for the year
    timeDate.year    = bcd2dec(Wire.read()) + 1970;
    timeDateBCD.year = dec2bcd(timeDate.year % 1000);
}


void Chronodot::readTime() {
    Wire.beginTransmission(DS3231_CTRL_ID);
    Wire.write((uint8_t)0x00);
    Wire.endTransmission();

    Wire.requestFrom(DS3231_CTRL_ID, 3);
    timeDateBCD.seconds = Wire.read() & 0b01111111; // ignore bit 7
    timeDateBCD.minutes = Wire.read();
    timeDateBCD.hours   = Wire.read() & 0b00111111; // ignore bit 6 and 7

    timeDate.seconds = bcd2dec(timeDateBCD.seconds);
    timeDate.minutes = bcd2dec(timeDateBCD.minutes);
    timeDate.hours   = bcd2dec(timeDateBCD.hours);
}


void Chronodot::readDate() {
    Wire.beginTransmission(DS3231_CTRL_ID);
    Wire.write((uint8_t)0x03);
    Wire.endTransmission();

    Wire.requestFrom(DS3231_CTRL_ID, 4);
    timeDateBCD.weekDay = Wire.read();
    timeDateBCD.day     = Wire.read();
    timeDateBCD.month   = Wire.read() & 0b01111111; // ignore bit 7

    timeDate.weekDay = bcd2dec(timeDateBCD.weekDay);
    timeDate.day     = bcd2dec(timeDateBCD.day);
    timeDate.month   = bcd2dec(timeDateBCD.month);

    // some special handling for the year
    timeDate.year    = bcd2dec(Wire.read()) + 1970;
    timeDateBCD.year = dec2bcd(timeDate.year % 1000);
}


void Chronodot::readTemperature() {
    //temp registers (11h-12h) get updated automatically every 64s
    Wire.beginTransmission(DS3231_CTRL_ID);
    Wire.write((uint8_t)0x11);
    Wire.endTransmission();
    Wire.requestFrom(DS3231_CTRL_ID, 2);

    uint8_t tempInt = Wire.read() & 0b01111111; // ignore bit 7
    uint8_t tempFraction = (Wire.read() >> 6) * 25; // only bit 7 and 8

    temperature = tempInt + (tempFraction / 100);

    temperatureIntBCD      = dec2bcd(tempInt);
    temperatureFractionBCD = dec2bcd(tempFraction);
}


void Chronodot::setSQW(int frequency) {
    // Frequency is stored in register 0x0e in bit 3 and 4
    Wire.beginTransmission(DS3231_CTRL_ID);
    Wire.write((uint8_t)0x0e);
    Wire.endTransmission();
    Wire.requestFrom(DS3231_CTRL_ID, 1);
    uint8_t register0E = Wire.read();
  

    // set frequency by changing bits 3 (RS1) and 4 (RS2)
    if (frequency == 1) {
        // clear bits 3 and 4
        register0E &= ~(1 << 3);
        register0E &= ~(1 << 4);
    } else if (frequency == 1024) {
        // bit 3 is 1, bit 4 is 0
        register0E |= 1 << 3;
        register0E &= ~(1 << 4);
    } else if (frequency == 4096) {
        // bit 3 is 0, bit 4 is 1
        register0E &= ~(1 << 3);
        register0E |= 1 << 4;
    } else if (frequency == 8192) {
        // set bits 3 and 4
        register0E |= 1 << 3;
        register0E |= 1 << 4;
    }

    // clear bit 2 (INTCN) to enable SWQ
    register0E &= ~(1 << 2);
  
    // put the value of the register back
    Wire.beginTransmission(DS3231_CTRL_ID);
    Wire.write((uint8_t)0x0e);
    Wire.write(register0E);
    Wire.endTransmission();
}


void Chronodot::setTimeDate(timeDateElements &tE) {
    Wire.beginTransmission(DS3231_CTRL_ID);
    Wire.write((uint8_t)0x00);
    Wire.write(dec2bcd(tE.seconds));
    Wire.write(dec2bcd(tE.minutes));
    Wire.write(dec2bcd(tE.hours));
    Wire.write(dec2bcd(tE.weekDay));
    Wire.write(dec2bcd(tE.day));
    Wire.write(dec2bcd(tE.month));
    Wire.write(dec2bcd(tE.year - 1970));
    Wire.endTransmission();
}


void Chronodot::setTime(timeDateElements &tE) {
    Wire.beginTransmission(DS3231_CTRL_ID);
    Wire.write((uint8_t)0x00);
    Wire.write(dec2bcd(tE.seconds));
    Wire.write(dec2bcd(tE.minutes));
    Wire.write(dec2bcd(tE.hours));
    Wire.endTransmission();
}


void Chronodot::setDate(timeDateElements &tE) {
    Wire.beginTransmission(DS3231_CTRL_ID);
    Wire.write((uint8_t)0x03);
    Wire.write(dec2bcd(tE.weekDay));
    Wire.write(dec2bcd(tE.day));
    Wire.write(dec2bcd(tE.month));
    Wire.write(dec2bcd(tE.year - 1970));
    Wire.endTransmission();
}

