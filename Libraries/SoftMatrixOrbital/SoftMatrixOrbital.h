#ifndef SoftMatrixOrbital_h
#define SoftMatrixOrbital_h

#include "WProgram.h"
#include "Print.h"
#include "../AFSoftSerial/AFSoftSerial.h"



class SoftMatrixOrbital : public Print {
    public:
        SoftMatrixOrbital(uint8_t rxPin, uint8_t txPin, int rows, int columns);
        void turnAutoScrollOn();
        void turnAutoScrollOff();
        void turnAutoLineWrapOn();
        void turnAutoLineWrapOff();
        void turnBlockCursorOn();
        void turnBlockCursorOff();
        void turnUnderlineCursorOn();
        void turnUnderlineCursorOff();
        void clear();
        void newLine();
        void cursorHome();
        void cursorSet(int x, int y);
        void backspace();
        void cursorLeft();
        void cursorRight();
        void setContrast(int contrast);
        void backlightOn(int minutes);
        void backlightOff();
    private:
        virtual void write(uint8_t b);
        AFSoftSerial _serial;
        int _rows;
        int _columns;
};

#endif

