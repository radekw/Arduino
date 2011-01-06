#include "WProgram.h"
#include "RGBColor.h"

RGBColor::RGBColor(int red, int green, int blue) {
    _red = constrain(red, 0, 100);
    _green = constrain(green, 0, 100);
    _blue = constrain(blue, 0, 100);
}


int RGBColor::getRed() {
    return _red;
}

int RGBColor::getGreen() {
    return _green;
}

int RGBColor::getBlue() {
    return _blue;
}

void RGBColor::setRed(int red) {
    _red = constrain(red, 0, 100);
}

void RGBColor::setGreen(int green) {
    _green = constrain(green, 0, 100);
}

void RGBColor::setBlue(int blue) {
    _blue = constrain(blue, 0, 100);
}

