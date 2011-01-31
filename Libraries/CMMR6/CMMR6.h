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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with CMMR6 Library. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef CMMR6_h
#define CMMR6_h

class CMMR6 {
    public:
        CMMR6();

        void handleInterrupt(int pinValue);

        void reset(void);
        void readChange(void);
        void translateFrameBuffer(void);
        void getTimeFromFrameBuffer(void);

        volatile byte lineState; // current state of the WWVB signal
        byte previousLineState;  // previous state of the WWVB signal

        int pulseLength;
        int frameError;
        int bitValue;

        int frameMark;
        int positionMarkCount;

        int hours;
        int minutes;
        int dayOfYear;
        int day;
        int month;
        int year;
        int leapYear;
        int leapSecond;
        int daylightSavingTime;
        float ut1;

        int timeReady;
        int gotFrame;

    private:
        int pulseStartMillis;
        int previousBitIsPositionMark;

        int frameBuffer[60];
        int bitCount;
};


#endif
