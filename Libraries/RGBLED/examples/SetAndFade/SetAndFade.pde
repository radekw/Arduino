#include <RGBLED.h>
#include <RGBColor.h>

/*
 * red pin 9; green pin 10, blue pin 11
 * hold 0ms, wait 2ms, common anode
*/
RGBLED rgbLED = RGBLED(9, 10, 11, 0, 2, 1);

RGBColor black = RGBColor(0, 0, 0);
RGBColor white = RGBColor(100, 100, 100);
RGBColor red   = RGBColor(100, 0, 0);
RGBColor green = RGBColor(0, 100, 0);
RGBColor blue  = RGBColor(0, 0, 100);

void setup() {
  rgbLED.setIntensity(100);
  rgbLED.setColor(black);
}

void loop() {
  rgbLED.crossfade(white);
  
  rgbLED.crossfade(red);
  
  rgbLED.crossfade(green);
  
  rgbLED.setCrossfadeHold(1);
  rgbLED.setCrossfadeWait(0);
  rgbLED.crossfade(blue);
  rgbLED.setCrossfadeHold(0);
  rgbLED.setCrossfadeWait(2);
}

