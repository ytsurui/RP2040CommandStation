#include "pico/stdlib.h"
#include "th_main.h"

#include "../../throttle/peripheral/lcd.h"

uint32_t throttleAppMain::bootMsgCounter = 0;

void throttleAppMain::bootVersionInfo()
{
    if (bootMsgCounter == 0) {
        clearLCD();
        setLCDpos(0, 0);
        writeLCDstring("MT40 DCC System");
        setLCDpos(1, 0);
        writeLCDstring("CommandStation1");
    } else if (bootMsgCounter == 1500) {
        setLCDpos(1, 0);
        writeLCDstring("FW Ver 0.9.2   ");
    }
}

uint8_t throttleAppMain::bootMsgShowTimming() {
    if (bootMsgCounter <= 3000) {
        bootMsgCounter++;
        if (bootMsgCounter == 1500) {
            return 1;
        }
        return 0;
    }

    return 0xFF;
}