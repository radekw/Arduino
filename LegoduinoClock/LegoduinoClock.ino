#include <Wire.h>
#include <Chronodot.h>
#include <SoftwareSerial.h>
#include <XBee.h>
#include <LED.h>
#include <MatrixDisplay.h>
#include <DisplayToolbox.h>
#include <EEPROM.h>
#include <string.h>
#include "font5x8.h"


/* ************************************************** */
#define ONE_HZ_PIN 3

#define coordinatorAddress64Msb 0x0013A200
#define coordinatorAddress64Lsb 0x406228FC

#define SETTINGS_START_ADDR 0
#define SETTINGS_IDENT_STR "set"


/* ************************************************** */
enum fonttype {
  FIVEBYEIGHT = 58,
  EIGHTBYEIGHT = 88
};

struct SettingsStore {
  uint8_t brightness;
  enum fonttype fontType;
} settings = { 15, FIVEBYEIGHT };


/* ************************************************** */
Chronodot chronodot = Chronodot();
LED statusLed = LED(13);
XBee xbee = XBee();
MatrixDisplay disp(2, 11, 10, false);
DisplayToolbox toolbox(&disp);
SoftwareSerial softSerial(8, 9);

volatile boolean displayNow = false;
boolean sendTimeNextSecond = false;
XBeeResponse response = XBeeResponse();
ZBRxResponse rx = ZBRxResponse();
ModemStatusResponse msr = ModemStatusResponse();
ZBTxStatusResponse txStatus = ZBTxStatusResponse();
XBeeAddress64 remoteAddress64 = XBeeAddress64(0x00000000, 0x00000000);
uint8_t X_MAX = 0;
uint8_t Y_MAX = 0;
char displayString[16];
char debugString[64];




/* ************************************************** */
void setup() {
  
  loadSettings();
  
  softSerial.begin(9600);
  xbee.setSerial(softSerial);
  xbee.begin(9600);

  X_MAX = disp.getDisplayCount() * (disp.getDisplayWidth() - 1) + 1;
  Y_MAX = disp.getDisplayHeight();
  disp.initDisplay(0, 4, true);
  disp.initDisplay(1, 5, false);
  disp.setBrightness(0, settings.brightness);
  disp.setBrightness(1, settings.brightness);

  chronodot.setSQW(1);

  // register interrupt function to 1Hz line
  pinMode(ONE_HZ_PIN, INPUT);
  digitalWrite(ONE_HZ_PIN, HIGH);
  attachInterrupt(1, oneHzInterruptHandler, FALLING);
}


/* ************************************************** */
void loop() {
  if (displayNow == true) {
    chronodot.readTimeDate();
    
    if (sendTimeNextSecond == true) {
      sendCurrentTime();
      sendTimeNextSecond = false;
    }
    
    displayTime();
    //displayBinaryTime();
    disp.syncDisplays();
    displayNow = false;
  }

  readXBee();
}


/* ************************************************** */
void oneHzInterruptHandler(void) {
  displayNow = true;
}


/* ************************************************** */
void displayTime() {
  sprintf(displayString, "%02u:%02u:%02u", chronodot.timeDate.hours, 
  chronodot.timeDate.minutes, chronodot.timeDate.seconds);
  drawString(8, 0, displayString);
}


/* ************************************************** */
void displayDate() {
  sprintf(displayString, "%04u-%02u-%02u", chronodot.timeDate.year, 
  chronodot.timeDate.month, chronodot.timeDate.day);
  drawString(2, 0, displayString);
}


/* ************************************************** */
void displayTemperature() {
  chronodot.readTemperature();
  sprintf(displayString, "%02.2f C", chronodot.temperature);
  drawString(1, 0, displayString);
}


/* ************************************************** */
void displayBinaryTime() {
  int x = 56;
  int y = 4;
  uint8_t firstSec = firstBCDdigit(chronodot.timeDateBCD.seconds);
  uint8_t secSec = secondBCDdigit(chronodot.timeDateBCD.seconds);
  uint8_t firstMin = firstBCDdigit(chronodot.timeDateBCD.minutes);
  uint8_t secMin = secondBCDdigit(chronodot.timeDateBCD.minutes);
  uint8_t firstHour = firstBCDdigit(chronodot.timeDateBCD.hours);
  uint8_t secHour = secondBCDdigit(chronodot.timeDateBCD.hours);

}


/* ************************************************** */
uint8_t firstBCDdigit(uint8_t num) {
  return num >> 4;
}


/* ************************************************** */
uint8_t secondBCDdigit(uint8_t num) {
  return num & 0b00001111;
}


/* ************************************************** */
void readXBee(void) {
  
  xbee.readPacket(10);

  if (xbee.getResponse().isAvailable()) {
    
    if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE) {
      xbee.getResponse().getZBRxResponse(rx);
      
      blinkLed(1, 10);
      uint8_t dataSize = rx.getDataLength();
      uint8_t* data = rx.getData();
      
      // read the command and decide what to do
      // "get..."
      if ((dataSize >= 3) && ((char)data[0] == 'g') && ((char)data[1] == 'e') && ((char)data[2] == 't')) {
//        if ((rx.getRemoteAddress64().getMsb() == coordinatorAddress64Msb) && 
//            (rx.getRemoteAddress64().getLsb() == coordinatorAddress64Lsb)) {
//            remoteAddress64.setMsb(0x00000000);
//            remoteAddress64.setLsb(0x00000000);
//          } else {
//            remoteAddress64.setMsb(rx.getRemoteAddress64().getMsb());
//            remoteAddress64.setLsb(rx.getRemoteAddress64().getLsb());
//          }
          remoteAddress64.setMsb(rx.getRemoteAddress64().getMsb());
          remoteAddress64.setLsb(rx.getRemoteAddress64().getLsb());
          // "get now"
          if ((dataSize == 7) && ((char)data[4] == 'n') && ((char)data[5] == 'o') && ((char)data[6] == 'w')) {
            sendCurrentTime();
          // "get" at the next second
          } else {
            sendTimeNextSecond = true;
          }
      // "set Tyyyymmddhhmmss"
      } else if ((dataSize == 19) && ((char)data[0] == 's') && ((char)data[1] == 'e') && ((char)data[2] == 't')) {
          readAndSetTime(dataSize, data);
      }
    }
  }
}


/* ************************************************** */
void readAndSetTime(uint8_t dataSize, uint8_t* data) {
  char inputString[20];
  for (int i = 0; i < 19; i++)
    inputString[i] = data[i];
  inputString[19] = '\0';
  int yr = 0, mo = 0, dy = 0, h = 0, m = 0, s = 0;
  int r = sscanf(inputString, "%*s T %4d %2d %2d %2d %2d %2d", &yr, &mo, &dy, &h, &m, &s);
  if ((r > 0) && (mo > 0) && (dy > 0)) {
    timeDateElements tE;
    tE.seconds = s;
    tE.minutes = m;
    tE.hours   = h;
    tE.weekDay = 0;
    tE.day     = dy;
    tE.month   = mo;
    tE.year    = yr;
    chronodot.setTimeDate(tE);
  }
}


/* ************************************************** */
void sendCurrentTime() {
  char dateTimeString[16];
  sprintf(displayString, "T%04u%02u%02u%02u%02u%02u", 
    chronodot.timeDate.year, chronodot.timeDate.month, chronodot.timeDate.day, 
    chronodot.timeDate.hours, chronodot.timeDate.minutes, chronodot.timeDate.seconds);
  ZBTxRequest zbTx = ZBTxRequest(remoteAddress64, (uint8_t*)displayString, 15);
  xbee.send(zbTx);
}


/* ************************************************** */
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


/* ************************************************** */
void drawChar(uint8_t x, uint8_t y, char c) {
  uint8_t dots;
  for (char col = 0; col < 5; col++) {
    dots = pgm_read_byte_near(&myfont[c][col]);
    for (char row = 0; row < 8; row++) {
      if (dots & (128 >> row))
        toolbox.setPixel(x+col, y+row, 1);
      else 
        toolbox.setPixel(x+col, y+row, 0);
    }
  }
}


/* ************************************************** */
void drawString(uint8_t x, uint8_t y, char* c) {
  for(char i = 0; i < strlen(c); i++)
  {
    drawChar(x, y, c[i]);
    x+=6; // Width of each glyph
  }
}


/* ************************************************** */
void loadSettings() {
  if (EEPROM.read(SETTINGS_START_ADDR + 0) == SETTINGS_IDENT_STR[0] &&
      EEPROM.read(SETTINGS_START_ADDR + 1) == SETTINGS_IDENT_STR[1] &&
      EEPROM.read(SETTINGS_START_ADDR + 2) == SETTINGS_IDENT_STR[2])
    for (unsigned int t = 0; t < sizeof(settings); t++)
      *((char*)&settings + t) = EEPROM.read(SETTINGS_START_ADDR + t);
}


/* ************************************************** */
void saveSettings() {
  for (unsigned int t = 0; t < sizeof(settings); t++)
    EEPROM.write(SETTINGS_START_ADDR + t, *((char*)&settings + t));
}


/* ************************************************** */

