/*
 * Copyright, 2011 Radek Wierzbicki
 *
 * This file is part of CMMR6 Library.
 *
 * CMMR6 Library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * CMMR6 Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with CMMR6 Library. If not, see <http://www.gnu.org/licenses/>.
*/

#include <WProgram.h>
#include "CMMR6.h"


int monthLengths[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};


CMMR6::CMMR6() {
    reset();
    timeReadyCallback = NULL;
    frameReadyCallback = NULL;
}


void CMMR6::reset(void) {
    lineState = 0;
    previousLineState = 0;

    pulseStartMillis = millis();
    pulseLength = 0;
    frameError = 1;
    bitValue = 0;

    frameMark = 0;
    positionMarkCount = 0;
    previousBitIsPositionMark = 0;

    for (int c = 0; c < 60; c++)
        frameBuffer[c] = 0;
    bitCount = 0;

    hours = 0;
    minutes = 0;
    dayOfYear = 0;
    day = 0;
    month = 0;
    year = 0;
    leapYear = 0;
    leapSecond = 0;
    daylightSavingTime = 0;
    ut1 = 0;
}


void CMMR6::handleInterrupt(int pinValue) {
    lineState = pinValue;
}


void CMMR6::readChange(void) {
    if (lineState == previousLineState) {
        return;
    }

    if (lineState == LOW) {
        pulseStartMillis = millis();
    } else {
        pulseLength = millis() - pulseStartMillis;
        
        // don't wanna overfill the buffer
        if (bitCount >= 60)
            bitCount = 0;
        
        
        // ignore less than 50ms
        if (pulseLength < 50) {
            ;
        
        // noise
        } else if (pulseLength < 150) {
            bitValue = 8;
            frameError++;
            previousBitIsPositionMark = 0;
            
        // 200ms pulse (150ms - 250ms)
        // logical 0
        } else if (pulseLength < 250) {
            bitValue = 0;
            previousBitIsPositionMark = 0;
            if (frameError == 0)
                frameBuffer[bitCount++] = bitValue;
            
        // noise
        } else if (pulseLength < 450) {
            bitValue = 8;
            frameError++;
            previousBitIsPositionMark = 0;
            
        // 500ms pulse (450ms - 550ms)
        // logical 1
        } else if (pulseLength < 550) {
            bitValue = 1;
            previousBitIsPositionMark = 0;
            if (frameError == 0)
                frameBuffer[bitCount++] = bitValue;
            
        //noise
        } else if (pulseLength < 750) {
            bitValue = 8;
            frameError++;
            previousBitIsPositionMark = 0;
            
        // 800ms pulse (750ms - 850ms)
        // mark pulse
        } else if (pulseLength < 850) {
            bitValue = 2;
            
            // this is the beginning of the new frame
            if (previousBitIsPositionMark == 1) {
                frameMark = 1;
                frameError = 0;
                
                bitCount = 0;
                positionMarkCount = 0;
            
            // just another marker
            } else {
                if (positionMarkCount >=6)
                    positionMarkCount = 0; // didn't get the frame mark
                else
                    positionMarkCount++;
                frameMark = 0;
            }
            
            // should be able to read the time after the second position marker
            if ((frameError == 0) && (positionMarkCount == 2) && (bitCount == 19)) {
                getTimeFromFrameBuffer();
                if (timeReadyCallback != NULL)
                    (*timeReadyCallback)();
            }

            // we had a good frame
            else if ((frameError == 0) && (positionMarkCount == 6) && (bitCount == 59)) {
                translateFrameBuffer();
                if (frameReadyCallback != NULL)
                    (*frameReadyCallback)();
            }

            
            previousBitIsPositionMark = 1;
            if (frameError == 0)
                frameBuffer[bitCount++] = bitValue;
        
        // noise
        } else {
            bitValue = 8;
            frameError++;
            previousBitIsPositionMark = 0;
        }
        
        
    }
    previousLineState = lineState;
}


void CMMR6::translateFrameBuffer(void) {
    getTimeFromFrameBuffer();

    dayOfYear  = 200 * frameBuffer[22];
    dayOfYear += 100 * frameBuffer[23];
    dayOfYear +=  80 * frameBuffer[25];
    dayOfYear +=  40 * frameBuffer[26];
    dayOfYear +=  20 * frameBuffer[27];
    dayOfYear +=  10 * frameBuffer[28];
    dayOfYear +=   8 * frameBuffer[30];
    dayOfYear +=   4 * frameBuffer[31];
    dayOfYear +=   2 * frameBuffer[32];
    dayOfYear +=   1 * frameBuffer[33];
    if (dayOfYear > 366) {
        frameError++;
        dayOfYear = 0;
    }

    year  = 80 * frameBuffer[45];
    year += 40 * frameBuffer[46];
    year += 20 * frameBuffer[47];
    year += 10 * frameBuffer[48];
    year +=  8 * frameBuffer[50];
    year +=  4 * frameBuffer[51];
    year +=  2 * frameBuffer[52];
    year +=  1 * frameBuffer[53];
    if (year > 99) {
        frameError++;
        year = 0;
    } else {
        year += 2000;
    }

    leapYear = frameBuffer[55];
    leapSecond = frameBuffer[56];

    // 0 - standard time
    // 1 - change from ST to DST
    // 2 - change from DST to ST
    // 3 - daylight saving time
    daylightSavingTime = (2 * frameBuffer[57]) + frameBuffer[58];

    // calculate month and day of the month from day of the year
    if (leapYear == 1)
        monthLengths[1] = 29;
    else
        monthLengths[1] = 28;
    int monthStart = 0;
    for (int m = 0; m < 12; m++) {
        if ((dayOfYear > monthStart) && (dayOfYear <= (monthStart + monthLengths[m]))) {
            day = dayOfYear - monthStart;
            month = m + 1;
            break;
        }
        monthStart += monthLengths[m];
    }

    ut1  = 0.8 * frameBuffer[40];
    ut1 += 0.4 * frameBuffer[41];
    ut1 += 0.2 * frameBuffer[42];
    ut1 += 0.1 * frameBuffer[43];
    if ((ut1 > 0.8) or (ut1 < -0.8)) {
        frameError++;
        ut1 = 0;
    }
    if ((frameBuffer[36] == 1) && (frameBuffer[37] == 0) && (frameBuffer[38] == 1))
        ut1 *= 1;
    else if ((frameBuffer[36] == 0) && (frameBuffer[37] == 1) && (frameBuffer[38] == 0))
        ut1 += -1;
    else {
        frameError++;
        ut1 == 0;
    }
}


void CMMR6::getTimeFromFrameBuffer(void) {
    minutes  = 40 * frameBuffer[1];
    minutes += 20 * frameBuffer[2];
    minutes += 10 * frameBuffer[3];
    minutes +=  8 * frameBuffer[5];
    minutes +=  4 * frameBuffer[6];
    minutes +=  2 * frameBuffer[7];
    minutes +=  1 * frameBuffer[8];
    if (minutes > 59) {
        frameError++;
        minutes = 0;
    }

    hours  = 20 * frameBuffer[12];
    hours += 10 * frameBuffer[13];
    hours +=  8 * frameBuffer[15];
    hours +=  4 * frameBuffer[16];
    hours +=  2 * frameBuffer[17];
    hours +=  1 * frameBuffer[18];
    if (hours > 23) {
        frameError++;
        hours = 0;
    }
}


void CMMR6::attachTimeReadyCallback(callbackFunction fn) {
    timeReadyCallback = fn;
}


void CMMR6::attachFrameReadyCallback(callbackFunction fn) {
    frameReadyCallback = fn;
}

