#ifndef RGBColor_h
#define RGBColor_h

#include "WProgram.h"

class RGBColor {
    public:
        RGBColor(int red, int green, int blue);
        int getRed();
        int getGreen();
        int getBlue();
        void setRed(int red);
        void setGreen(int green);
        void setBlue(int blue);
    private:
        int _red;
        int _green;
        int _blue;
};

#endif

