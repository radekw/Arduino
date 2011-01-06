#include <RGBLED.h>
#include <RGBColor.h>
#include <LED.h>

static const byte CMD_START = 250;
static const byte CMD_END = 251;
static const byte CMD_SET_INTENSITY = 10;
static const byte CMD_SET_COLOR = 20;
static const byte CMD_CROSSFADE = 30;
static const byte CMD_SET_CROSSFADE_WAIT = 31;
static const byte CMD_SET_CROSSFADE_HOLD = 32;


RGBLED rgbLED = RGBLED(9, 10, 11, 0, 2, 1);
LED greenLed = LED(6);
LED redLed = LED(7);
int piezoPin = 12;
byte cmdbuf[32];


void setup() {
  Serial.begin(9600);
  pinMode(piezoPin, OUTPUT);
  RGBColor black = RGBColor(0, 0, 0);
  rgbLED.setIntensity(100);
  rgbLED.setColor(black);
  buzz();
  blinkRed();
  blinkGreen();
  buzz();
}


void loop() {
  int cmd, i, b;
  
  cmdbuf[0] = CMD_END;
  i = -1;
  while (Serial.available()) {
    b = Serial.read();
    if (i < 0) {
      if (b == CMD_START) {
        i = 0;
      }
    }
    if (i >= 0 && i < 32) {
      cmdbuf[i++] = b;
      blinkGreen();
      if (b == CMD_END) {
        break;
      }
    } else {
      blinkRed();
      continue;
    }
  }
  
  i = 0;
  if(cmdbuf[i++] == CMD_START) {
    cmd = cmdbuf[i++];
    if (cmd == CMD_SET_INTENSITY) {
      int intensity = cmdbuf[i++];
      buzz();
      blinkGreen();
      rgbLED.setIntensity(intensity);
    } else if (cmd == CMD_SET_COLOR) {
      RGBColor c = RGBColor(cmdbuf[i++], cmdbuf[i++], cmdbuf[i++]);
      buzz();
      blinkGreen();
      rgbLED.setColor(c);
    } else if (cmd == CMD_CROSSFADE) {
      RGBColor c = RGBColor(cmdbuf[i++], cmdbuf[i++], cmdbuf[i++]);
      buzz();
      blinkGreen();
      rgbLED.crossfade(c);
    } else if (cmd == CMD_SET_CROSSFADE_WAIT) {
      int wait = cmdbuf[i++];
      buzz();
      blinkGreen();
      rgbLED.setCrossfadeWait(wait);
    } else if (cmd == CMD_SET_CROSSFADE_HOLD) {
      int hold = cmdbuf[i++];
      buzz();
      blinkGreen();
      rgbLED.setCrossfadeHold(hold);
    } else {
      buzz();
      blinkRed();
      buzz();
      cmd = 0;
    }
  }
  
  delay(100);
}


void blinkGreen() {
  greenLed.on();
  delay(10);
  greenLed.off();
  delay(10);
}


void blinkRed() {
  redLed.on();
  delay(10);
  redLed.off();
  delay(10);
}


void buzz() {
  digitalWrite(piezoPin, HIGH);
  delay(5);
  digitalWrite(piezoPin, LOW);
}


