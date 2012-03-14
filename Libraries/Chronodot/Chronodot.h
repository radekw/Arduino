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

#ifndef Chronodot_h
#define Chronodot_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif


typedef struct { 
    uint8_t seconds; 
    uint8_t minutes; 
    uint8_t hours; 
    uint8_t weekDay;
    uint8_t day;
    uint8_t month; 
    uint16_t year;
} timeDateElements;

uint8_t dec2bcd(uint8_t num);
uint8_t bcd2dec(uint8_t num);

class Chronodot {
    public:
        Chronodot();
        void readTimeDate();
        void readTime();
        void readDate();
        void readTemperature();
        void setSQW(int frequency);
        void setTimeDate(timeDateElements &tE);
        void setTime(timeDateElements &tE);
        void setDate(timeDateElements &tE);

        timeDateElements timeDateBCD;
        timeDateElements timeDate;

        float temperature;
        uint8_t temperatureIntBCD;
        uint8_t temperatureFractionBCD;
};

#endif
