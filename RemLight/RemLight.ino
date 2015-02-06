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
#include <SoftwareSerial.h>
#include <XBee.h>

// this is a hack to make pre-processor work
// for debugging on the LCD
// have to declare all my functions
void saveToEEPROM(void);
void readFromEEPROM(void);
void blinkLed();


// this turns the EEPROM saving and reading on
#define USE_EEEPROM 1

// this turns status/error LED
#define USE_STATUS_LED 0

#if USE_EEEPROM == 1
#include <EEPROM.h>
boolean needEEPROMSave = false;
unsigned long lastEEPROMSave = 0;
#endif


#if USE_STATUS_LED == 1
#include <LED.h>
LED statusLed = LED(13);
#endif


// Analog pin number for IR proximity sensor
#define PIN_PROXIMITY 3


/*
 * About the protocol:
 * The payload starts with a command type byte followed by a number of 
 * necessary parameters. Some commands have an undefined number of 
 * parameters. 
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
 *     Gets a single unsigned integer representing the wait time in 
 *     miliseconds that controlls the speed of the crossfade. 
 *     0 is the fastest.
 *   Set crossfade hold:
 *     Gets a single unsigned integer representing the wait time in
 *     seconds before the crossfade to the next color, if any.
 */


/* 
 * protocol constants
 */
static const byte CMD_SET_COLOR          = 20;
static const byte CMD_CROSSFADE          = 30;
static const byte CMD_SET_CROSSFADE_WAIT = 31;
static const byte CMD_SET_CROSSFADE_HOLD = 32;


/*
 * 1 megabrite
 * hold 0ms, wait 2ms, common anode
 */
Brites brites = Brites(3, 500, 25);
BriteColor black = BriteColor(0, 0, 0);


/* 
 * XBee objects
 */
XBee xbee = XBee();
XBeeResponse response = XBeeResponse();
ZBRxResponse rx = ZBRxResponse();
ModemStatusResponse msr = ModemStatusResponse();


/* 
 * Variables to keep colors and commands
 */
BriteColor *colors = NULL;
int numberOfColors = 0;
int currentColorIndex = 0;
int colorCmd = 0;

/*
 * Lamp on/off
 */
int lightOn = 1;



// setup function
void setup() {

  // initialize XBee
  xbee.begin(9600);
  
  // initialize brites
  BriteColor black = BriteColor(0, 0, 0);
  brites.setColor(black);

  #if USE_STATUS_LED == 1
  blinkLed(10, 100);
  #endif
  
  // read saved data from EEPROM
  #if USE_EEEPROM == 1
  readFromEEPROM();
  #endif
}


// main never ending loop
void loop() {
  
  xbee.readPacket();

  if (xbee.getResponse().isAvailable()) {

    if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
      xbee.getResponse().getZBRxResponse(rx);

      #if USE_STATUS_LED == 1
      if (rx.getOption() == ZB_PACKET_ACKNOWLEDGED) {
        blinkLed(10, 100); // status
      } else {
        blinkLed(2, 100); // error
      }
      #endif

      // analyze the payload and decide what to do
      uint8_t dataSize = rx.getDataLength();
      uint8_t* data = rx.getData();
      int dataIndex = 0;
      if (dataSize > 0) {
        int cmd = data[dataIndex++];
        
        // set the color
        // allocate colors array to one color and load it from payload
        // set the command, array length, and reset the index
        if (cmd == CMD_SET_COLOR && dataSize > 3) {
          if (colors != NULL) {
            free(colors);
          }
          colorCmd = CMD_SET_COLOR;
          currentColorIndex = 0;
          numberOfColors = 1;
          colors = (BriteColor*)malloc(1 * sizeof(BriteColor));
          colors[0] = BriteColor(data[dataIndex++], data[dataIndex++], data[dataIndex++]);
          #if USE_STATUS_LED == 1
          blinkLed(1, 100);
          #endif
          #if USE_EEEPROM == 1
          needEEPROMSave = true;
          #endif
    
          // set the crossfade
          // allocate colors array and set the command
          // read all the colors from the payload, allocate the colors array, 
          // and load colors into it
          // set the command, array length, and reset the index
        } else if (cmd == CMD_CROSSFADE && dataSize > 3) {
          if (colors != NULL) {
            free(colors);
          }
          colorCmd = CMD_CROSSFADE;
          currentColorIndex = 0;
          numberOfColors = (dataSize - 1) / 3;
          colors = (BriteColor*)malloc(numberOfColors * sizeof(BriteColor));
          for (int n = 0; n < numberOfColors; n++) {
            colors[n] = BriteColor(data[dataIndex++], data[dataIndex++], data[dataIndex++]);
          }
          #if USE_STATUS_LED == 1
          blinkLed(1, 100);
          #endif
          #if USE_EEEPROM == 1
          needEEPROMSave = true;
          #endif
    
        // set the crossfade wait
        } else if (cmd == CMD_SET_CROSSFADE_WAIT && dataSize > 2) {
          int vH = data[dataIndex++];
          int vL = data[dataIndex++];
          unsigned int v = (vH * 255) + vL;
          brites.setCrossfadeWait(v);
          #if USE_STATUS_LED == 1
          blinkLed(1, 100);
          #endif
          #if USE_EEEPROM == 1
          needEEPROMSave = true;
          #endif
    
        // set the crossfade hold
        } else if (cmd == CMD_SET_CROSSFADE_HOLD && dataSize > 2) {
          int vH = data[dataIndex++];
          int vL = data[dataIndex++];
          unsigned int v = (vH * 255) + vL;
          brites.setCrossfadeHold(v);
          #if USE_STATUS_LED == 1
          blinkLed(1, 100);
          #endif
          #if USE_EEEPROM == 1
          needEEPROMSave = true;
          #endif
    
        // everything else is crap
        } else {
          cmd = 0;
          #if USE_STATUS_LED == 1
          blinkLed(2, 100); // error
          #endif
          #if USE_EEEPROM == 1
          needEEPROMSave = true;
          #endif
        }
      }

    } else if (xbee.getResponse().getApiId() == MODEM_STATUS_RESPONSE) {
      xbee.getResponse().getModemStatusResponse(msr);

      #if USE_STATUS_LED == 1
      if (msr.getStatus() == ASSOCIATED) {
        blinkLed(5, 100); // status
      } else if (msr.getStatus() == DISASSOCIATED) {
        blinkLed(2, 100); // error
      } else {
        blinkLed(5, 100); // status
      }
      #endif

    } else {
      #if USE_STATUS_LED == 1
      blinkLed(2, 100); // error
      #endif
    }
  }
  
  // read proximity sensor
  int proximity = analogRead(PIN_PROXIMITY);
  if (proximity > 100) {
    
    int ptime = 0;
    while (proximity > 100) {
      delay(100);
      ptime += 100;
      if (ptime > 30000)
        break;
      proximity = analogRead(PIN_PROXIMITY);
    }
    
    if (ptime >= 200 && ptime < 30000) {
      #if USE_STATUS_LED == 1
      blinkLed(2, 250);
      #endif
      if (lightOn == 0) {
        lightOn = 1;
      } else {
        lightOn = 0;
      }
      currentColorIndex = 0;
    }
  }
  
  if (lightOn == 0) {
    brites.setColor(black);
  } else {
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
  }
  
  #if USE_EEEPROM == 1
  if (needEEPROMSave == true)
    saveToEEPROM();
  #endif
  
  // sleep some
  delay(100);
}


#if USE_EEEPROM == 1
void saveToEEPROM(void) {
  // dont' save if command change in less than 15 seconds
  if ((millis() - lastEEPROMSave) < 15000) {
    return;
  }
  
  int a = 0;
  EEPROM.write(a++, colorCmd);
  EEPROM.write(a++, numberOfColors);
  for (int n = 0; n < numberOfColors; n++) {
    BriteColor c = colors[n];
    EEPROM.write(a++, c.getRed());
    EEPROM.write(a++, c.getGreen());
    EEPROM.write(a++, c.getBlue());
  }
  EEPROM.write(a++, brites.getCrossfadeWait());
  EEPROM.write(a++, brites.getCrossfadeHold());
  
  lastEEPROMSave = millis();
  needEEPROMSave = false;
}
#endif


#if USE_EEEPROM == 1
void readFromEEPROM(void) {
  int a = 0;
  if (colors != NULL) {
      free(colors);
    }
    colorCmd = EEPROM.read(a++);
    if (colorCmd == 0 || colorCmd == 255)
      return;
    currentColorIndex = 0;
    numberOfColors = EEPROM.read(a++);
    colors = (BriteColor*)malloc(numberOfColors * sizeof(BriteColor));
    for (int n = 0; n < numberOfColors; n++) {
      colors[n] = BriteColor(EEPROM.read(a++), EEPROM.read(a++), EEPROM.read(a++));
    }
    brites.setCrossfadeWait(EEPROM.read(a++));
    brites.setCrossfadeHold(EEPROM.read(a++));
}
#endif


#if USE_STATUS_LED == 1
// blink the status led
void blinkLed(int times, int wait) {
  for (int i = 0; i < times; i++) {
    statusLed.on();
    delay(wait);
    statusLed.off();
    if (i + 1 < times) {
      delay(wait);
    }
  }
}
#endif

