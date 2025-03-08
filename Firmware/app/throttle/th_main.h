

#include "th_trainctrl.h"

class throttleAppMain
{
public:
    static void init();
    static void event();
    static void task();
    static void dispInit();
    static void dispEvent();
    static void dispTerminate();

    static void bootVersionInfo();
    static uint8_t bootMsgShowTimming();

    static void bootWirelessInitMsg(uint8_t devType);
    static void bootWirelessInfoMsg();
    static void bootWirelessSetNetIDMsg(uint32_t netID);

private:
    static throttleAppTrainCtrl trainCtrlObj[2];

    static bool showFlag;

    static void (*eventRoute)(void);
    static void (*keypadCb)(uint8_t);
    static void (*dispEventCb)(void);

    static void trainCtrl0KeypadEvent(uint8_t keycode);
    static void trainCtrl1KeypadEvent(uint8_t keycode);

    static void trainCtrl0DispEvent();
    static void trainCtrl1DispEvent();

    static uint16_t showPageID;

    static uint8_t eventCount;

    static uint32_t bootMsgCounter;
};

#define PAGE_ID_TOP 0x0000

#define PAGE_ID_TRAIN_0 0x0010
#define PAGE_ID_TRAIN_1 0x0011

#define PAGE_ID_SWITCH 0x0040
#define PAGE_ID_PROG 0x0080