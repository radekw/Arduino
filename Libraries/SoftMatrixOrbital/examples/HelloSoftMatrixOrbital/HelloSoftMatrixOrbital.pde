#include <AFSoftSerial.h>
#include <SoftMatrixOrbital.h>


// rx: 3; tx: 2
SoftMatrixOrbital lcd = SoftMatrixOrbital(3, 2, 2, 20);

void setup() {
  lcd.turnBlockCursorOff();
  lcd.turnUnderlineCursorOff();
  lcd.backlightOff();
  lcd.clear();
  delay(500);
  lcd.backlightOn(0);
}


void loop()
{
  lcd.clear();
  delay(500);
  
  lcd.print("Soft Matrix Orbital");
  lcd.newLine();
  delay(500);
  lcd.print("Hello");
  lcd.newLine();
  delay(1000);
}


