#include "WProgram.h"
#include "RGBLED.h"
#include "RGBColor.h"

/* 
* Library by Radek Wierzbicki
* Based on ColorCrossfader sketch by Clay Shirky <clay.shirky@nyu.edu>
* http://www.arduino.cc/en/Tutorial/ColorCrossfader
* 
* The library works with common anode and common cathode LED's
* It will use delays from TimeAlarms library if used
* http://www.arduino.cc/playground/Code/Time
* 
* 
* The program works like this:
* Imagine a crossfade that moves the red LED from 0-10, 
*   the green from 0-5, and the blue from 10 to 7, in
*   ten steps.
*   We'd want to count the 10 steps and increase or 
*   decrease color values in evenly stepped increments.
*   Imagine a + indicates raising a value by 1, and a -
*   equals lowering it. Our 10 step fade would look like:
* 
*   1 2 3 4 5 6 7 8 9 10
* R + + + + + + + + + +
* G   +   +   +   +   +
* B     -     -     -
* 
* The red rises from 0 to 10 in ten steps, the green from 
* 0-5 in 5 steps, and the blue falls from 10 to 7 in three steps.
* 
* In the real program, the color percentages are converted to 
* 0-255 values, and there are 1020 steps (255*4).
* 
* To figure out how big a step there should be between one up- or
* down-tick of one of the LED values, we call _calculateStep(), 
* which calculates the absolute gap between the start and end values, 
* and then divides that gap by 1020 to determine the size of the step  
* between adjustments in the value.
*/



/*
* Constructor
* redPid - analog output pin to drive red color
* greenPin - analog output pin to drive green color
* bluePin - analog output pin to drive blue color
* hold - internal crossfade delay in miliseconds; increase for slower fades
* wait - hold the color after the fade for X miliseconds
* commonAnode - set to 1 when using common anode LED, 0 for common cathode
*/
RGBLED::RGBLED(int redPin, int greenPin, int bluePin, 
               int hold, int wait, int commonAnode) {
  _redPin = redPin;
  _greenPin = greenPin;
  _bluePin = bluePin;
  _hold = hold;
  _wait = wait;
  _commonAnode = commonAnode;
  _intensity = 100;

  _redValue = 0;
  _greenValue = 0;
  _blueValue = 0;

  _prevRed = 0;
  _prevGreen = 0;
  _prevBlue = 0;

  pinMode(_redPin, OUTPUT);
  pinMode(_greenPin, OUTPUT);   
  pinMode(_bluePin, OUTPUT); 
}



/* crossFade() converts the percentage colors to a 
*  0-255 range, then loops 1020 times, checking to see if  
*  the value needs to be updated each time, then writing
*  the color values to the correct pins.
*/
void RGBLED::crossfade(RGBColor &color) {
  // map color to values from 0 to 255
  int R = _mapColor(color.getRed());
  int G = _mapColor(color.getGreen());
  int B = _mapColor(color.getBlue());
  
  int stepR = _calculateStep(_prevRed, R);
  int stepG = _calculateStep(_prevGreen, G); 
  int stepB = _calculateStep(_prevBlue, B);

  for (int i = 0; i <= 1020; i++) {
    _redValue = _calculateVal(stepR, _redValue, i);
    _greenValue = _calculateVal(stepG, _greenValue, i);
    _blueValue = _calculateVal(stepB, _blueValue, i);

    // Write current values to LED pins
    analogWrite(_redPin, _redValue);
    analogWrite(_greenPin, _greenValue);      
    analogWrite(_bluePin, _blueValue); 

    // Pause for 'wait' milliseconds before resuming the loop
#ifdef TimeAlarms_h    
    Alarm.delay(_wait);
#else
    delay(_wait);
#endif
  }
  // Update current values for next loop
  _prevRed = _redValue; 
  _prevGreen = _greenValue; 
  _prevBlue = _blueValue;
  // Pause for optional 'wait' milliseconds before resuming the loop
#ifdef TimeAlarms_h    
    Alarm.delay(_hold);
#else
    delay(_hold);
#endif
}



/*
* setColor() sets a color without crossfade
*/
void RGBLED::setColor(RGBColor &color) {
  _redValue = _mapColor(color.getRed());
  _greenValue = _mapColor(color.getGreen());
  _blueValue = _mapColor(color.getBlue());
  
  analogWrite(_redPin, _redValue);
  analogWrite(_greenPin, _greenValue);      
  analogWrite(_bluePin, _blueValue); 

  _prevRed = _redValue; 
  _prevGreen = _greenValue; 
  _prevBlue = _blueValue;

}


/*
* set the intensity of the LED
* accepts an integer from 0 to 100
*/
void RGBLED::setIntensity(int intensity) {
    _intensity = (float)constrain(intensity, 0, 100);
}



/*
* 
*/
void RGBLED::setCrossfadeWait(int wait) {
    _wait = wait;
}



/*
* 
*/
void RGBLED::setCrossfadeHold(int hold) {
    _hold = hold;
}



/*
* 
*/
int RGBLED::_calculateStep(int prevValue, int endValue) {
  int step = endValue - prevValue; // What's the overall gap?
  if (step) {                      // If its non-zero, 
    step = (1020) / step;          // divide by 1020
  } 
  return step;
}



/* When the loop value, i,
*  reaches the step size appropriate for one of the
*  colors, it increases or decreases the value of that color by 1. 
*  (R, G, and B are each calculated separately.)
*/
int RGBLED::_calculateVal(int step, int val, int i) {
  if ((step) && i % step == 0) { // If step is non-zero and its time to change a value,
    if (step > 0) {              // increment the value if step is positive...
      val += 1;           
    } else if (step < 0) {       // ...or decrement it if step is negative
      val -= 1;
    } 
  }
  // Defensive driving: make sure val stays in the range 0-255 given the intensity
  int min;
  int max;
  if (_commonAnode == 0) {
    min = 0;
    max = 255 * (_intensity / 100);
  } else {
    min = 255 - (255 * (_intensity / 100));
    max = 255;
  }
  val = constrain(val, min, max);
  return val;
}



/* _mapColor() maps color described by value from 0 to 100
* to value from 0 to 255 accepted by analogWrite
* taking intensity into account
*/
int RGBLED::_mapColor(int color) {
  int C;
  if (_commonAnode == 0) {
    C = ((color * 255) / 100);
    C = map(C, 0, 255, 0, 255 * (_intensity / 100));
  } else {
    C = (((100 - color) * 255) / 100);
    C = map(C, 0, 255, 255 - (255 * (_intensity / 100)), 255);
  }
  return C;
}



