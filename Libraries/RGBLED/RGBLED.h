#ifndef RGBLED_h
#define RGBLED_h

#include "WProgram.h"
#include "RGBColor.h"

class RGBLED {
    public:
        RGBLED(int redPin, int greenPin, int bluePin, 
               int hold, int wait, int commonAnode);
        void setup();
        void crossfade(RGBColor &color);
        void setColor(RGBColor &color);
        void setIntensity(int intensity);
        void setCrossfadeWait(int wait);
        void setCrossfadeHold(int hold);
    private:
        int _commonAnode;

        int _hold;
        int _wait;

        float _intensity;

        int _redPin;
        int _greenPin;
        int _bluePin;

        int _redValue;
        int _greenValue;
        int _blueValue;

        int _prevRed;
        int _prevGreen;
        int _prevBlue;

        int _calculateStep(int prevValue, int endValue);
        int _calculateVal(int step, int val, int i);
        int _mapColor(int color);
};

#endif

