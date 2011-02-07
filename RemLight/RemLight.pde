/*
 * Copyright, 2011 Radek Wierzbicki
 *  
 * This file is part of RemLight.
 *
 * RemLight is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * RemLight is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with RemLight.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <Brites.h>
#include <BriteColor.h>
#include <LED.h>

// this is a hack to make pre-processor work
// for debugging on the LCD
// have to declare all my functions
void blinkGreen();
void blinkRed();

// this turns the LCD debugging on
// set to 0 to disable
#define DEBUG 0

#if DEBUG == 1
  // LCD for debugging
  #include <AFSoftSerial.h>
  #include <SoftMatrixOrbital.h>
  // rx: 3; tx: 2
  SoftMatrixOrbital lcd = SoftMatrixOrbital(3, 2, 2, 20);
#endif


/*
 * About the protocol:
 * Each packet starts with a start byte followed by the data size byte
 * The payload starts at the third byte with a command type byte
 * followed by a number of necessary parameters. Some commands have an
 * undefined number of parameters. 
 * Color definitions:
 *   Color is defined by three bytes, each representing color intensity
 *   of one color (namely, red, green, and blue). Intensity is specified
 *   by a number between 0 and 100: 0 for the lowest intensity. Let's
 *   call that a color tripplet.
 * Folowing commands are defined:
 *   Set color: 
 *     Must get a color tripplet (3 bytes)
 *   Crossfade:
 *     Gets any number of color tripplets. It will crossfade continously
 *     between all given colors.
 *   Set crossfade wait:
 *     Gets a single 8-bit integer representing the wait time in 
 *     miliseconds that controlls the speed of the crossfade. 
 *     0 is the fastest.
 *   Set crossfade hold:
 *     Gets a single 8-bit integer representing the wait time in seconds
 *     before the crossfade to the next color, if any.
*/

// Here are the protocol constants
static const byte CMD_START              = 250;
static const byte CMD_SET_COLOR          = 20;
static const byte CMD_CROSSFADE          = 30;
static const byte CMD_SET_CROSSFADE_WAIT = 31;
static const byte CMD_SET_CROSSFADE_HOLD = 32;

/*
 * 1 megabrite
 * hold 0ms, wait 2ms, common anode
 */
Brites brites = Brites(3, 0, 2);

// Define the hardware outputs - some for debugging
LED greenLed  = LED(6);
LED redLed    = LED(7);

// setup function
void setup() {
  
  // open serial
  Serial.begin(9600);
  
  // initialize brites
  BriteColor black = BriteColor(0, 0, 0);
  brites.setColor(black);
  
  // startup song
  blinkRed();
  blinkGreen();
  
  #if DEBUG == 1
  // setup LCD for debugging
  lcd.turnBlockCursorOff();
  lcd.turnUnderlineCursorOff();
  lcd.clear();
  lcd.backlightOn(0);
  lcd.cursorHome();
  #endif
}

// Variables to keep colors and commands
BriteColor *colors = NULL;
int numberOfColors = 0;
int currentColorIndex = 0;
int colorCmd = 0;


// main never ending loop
void loop() {
  int i = -1;
  int b = 0;
  int datasize = 0;
  
  byte *cmdbuf = NULL;
  
  // read the serial buffer until no more data
  i = -1;
  while (Serial.available()) {
    // get one byte
    b = Serial.read();
    
    // on the first byte, check if it's the CMD_START
    if (i < 0) {
      if (b == CMD_START) {
        i = 0;
        blinkGreen();
        #if DEBUG == 1
        lcd.clear();
        lcd.cursorHome();
        #endif
      } else {
        blinkRed();
      }
    
    // on the second byte, get the payload length
    // and allocate cmdbuf array to hold the payload
    } else if (i == 0) {
      datasize = (int)b;
      cmdbuf = (byte*)malloc(datasize * sizeof(byte));
      for (int j = 0; j < datasize; j++) {
        cmdbuf[j] = (byte)0;
      }
      i++;
      blinkGreen();
      #if DEBUG == 1
      lcd.print(b);
      lcd.print(" ");
      #endif
      
    // the next datasize of bytes is the paylod
    // load it into the cmdbuf array
    } else if (i >= 1 && i <= datasize) {
      int j = i - 1;
      cmdbuf[j] = (byte)b;
      i++;
      blinkGreen();
      #if DEBUG == 1
      lcd.print(b);
      lcd.print(" ");
      #endif
      
    // the rest is crap - ignore
    } else {
      i++;
      blinkRed();
      #if DEBUG == 1
      lcd.print("e ");
      #endif
    }
  }
  
  
  // analyze the payload and decide what to do
  i = 0;
  if(cmdbuf != NULL) {
    // the fist byte is the command
    int cmd = cmdbuf[i++];
    
    // set the color
    // allocate colors array to one color and load it from payload
    // set the command, array length, and reset the index
    if (cmd == CMD_SET_COLOR) {
      if (colors != NULL) {
        free(colors);
      }
      colorCmd = CMD_SET_COLOR;
      currentColorIndex = 0;
      numberOfColors = 1;
      colors = (BriteColor*)malloc(1 * sizeof(BriteColor));
      colors[0] = BriteColor(cmdbuf[i++], cmdbuf[i++], cmdbuf[i++]);
      blinkGreen();
      
    // set the crossfade
    // allocate colors array and set the command
    // read all the colors from the payload, allocate the colors array, 
    // and load colors into it
    // set the command, array length, and reset the index
    } else if (cmd == CMD_CROSSFADE) {
      if (colors != NULL) {
        free(colors);
      }
      colorCmd = CMD_CROSSFADE;
      currentColorIndex = 0;
      numberOfColors = (datasize - 1) / 3;
      colors = (BriteColor*)malloc(numberOfColors * sizeof(BriteColor));
      for (int n = 0; n < numberOfColors; n++) {
        colors[n] = BriteColor(cmdbuf[i++], cmdbuf[i++], cmdbuf[i++]);
      }
      blinkGreen();

    // set the crossfade wait
    } else if (cmd == CMD_SET_CROSSFADE_WAIT) {
      int wait = cmdbuf[i++];
      brites.setCrossfadeWait(wait);
      blinkGreen();
      
    // set the crossfade hold
    } else if (cmd == CMD_SET_CROSSFADE_HOLD) {
      int hold = (int)cmdbuf[i++] * 1000;
      brites.setCrossfadeHold(hold);
      blinkGreen();
      
    // everything else is crap
    } else {
      cmd = 0;
      blinkRed();
    }
  }
  
  
  // do the colors according to the set command
  // and collors array
  if (colorCmd == CMD_SET_COLOR) {
    if (colors != NULL) {
      brites.setColor(colors[currentColorIndex]);
    }
  } else if (colorCmd == CMD_CROSSFADE) {
    if (colors != NULL) {
      brites.crossfade(colors[currentColorIndex]);
    }
    if (currentColorIndex < numberOfColors - 1) {
      currentColorIndex++;
    } else {
      currentColorIndex = 0;
    }
  }
  
  // free the cmdbuf array
  free(cmdbuf);
  
  // sleep some
  delay(100);
}


// blink the green led
void blinkGreen() {
  greenLed.on();
  delay(10);
  greenLed.off();
  delay(10);
}


// blink the red led
void blinkRed() {
  redLed.on();
  delay(10);
  redLed.off();
  delay(10);
}


