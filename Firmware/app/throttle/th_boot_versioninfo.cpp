#include "pico/stdlib.h"
#include "th_main.h"

#include "../../throttle/peripheral/lcd.h"
#include "../wconfig/wconfig.h"

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
        writeLCDstring("FW Ver 0.9.3   ");
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

void throttleAppMain::bootWirelessInitMsg(uint8_t devType)
{
    clearLCD();
    switch (devType) {
        case WIRELESS_CONFIG_MODULE_TYPE_TWELITE_UART:
            setLCDpos(0, 0);
            writeLCDstring("TWE UART Init...");
            break;
    }
}

void throttleAppMain::bootWirelessInfoMsg()
{
    clearLCD();
    if (wirelessConfig::getModuleType() == WIRELESS_CONFIG_MODULE_TYPE_TWELITE_UART) {
        setLCDpos(0, 0);
        writeLCDstring("TWE UART ");
        writeLCDstring(wirelessConfig::versionInfo);
        setLCDpos(1, 0);
        writeLCDstring("netID: ");
        showHex32Bit(wirelessConfig::twelite_app_id);

    } else if (wirelessConfig::getModuleType() == WIRELESS_CONFIG_MODULE_TYPE_TWELITE_UNKNOWN) {
        setLCDpos(0, 0);
        writeLCDstring("TWE-LITE Unknown");
    } else {
        setLCDpos(0, 0);
        writeLCDstring("Unknown Wireless");
    }
}

void throttleAppMain::bootWirelessSetNetIDMsg(uint32_t netID)
{
    clearLCD();
    setLCDpos(0, 0);
    writeLCDstring("change NetID: ");
    setLCDpos(1, 0);
    writeLCDstring("newID: ");
    showHex32Bit(netID);
}