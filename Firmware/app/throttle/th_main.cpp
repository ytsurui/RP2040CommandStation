
#include "pico/stdlib.h"
#include "th_main.h"
#include "th_switchctrl.h"
#include "th_prog.h"
#include "string.h"

#include "../../peripheral/adcReader.h"
#include "../current_monitor.h"
#include "../voltage_monitor.h"

#include "../../throttle/peripheral/lcd.h"
#include "../../throttle/peripheral/keypad.h"
#include "../../throttle/peripheral/direction.h"

bool throttleAppMain::showFlag;
throttleAppTrainCtrl throttleAppMain::trainCtrlObj[2];

void (*throttleAppMain::eventRoute)(void);
void (*throttleAppMain::keypadCb)(uint8_t);
void (*throttleAppMain::dispEventCb)(void);

uint16_t throttleAppMain::showPageID;
uint8_t throttleAppMain::eventCount;

void throttleAppMain::init()
{
    keypad::init();
    directionSW::init();
    initLCD();

    showFlag = true;

    trainCtrlObj[0].init('A');
    trainCtrlObj[1].init('B');

    eventRoute = nullptr;
    keypadCb = nullptr;
    dispEventCb = nullptr;

    showPageID = PAGE_ID_TOP;
    eventCount = 0;
}

void throttleAppMain::event()
{
    uint8_t volt;
    uint16_t currTmp;

    keypad::event();
    directionSW::event();

    trainCtrlObj[0].event();
    trainCtrlObj[1].event();

    trainCtrlObj[0].volumeEvent((readADCcachedValue(3) >> 5));
    trainCtrlObj[1].volumeEvent((readADCcachedValue(2) >> 5));

    dispEvent();

    eventCount++;
    if (eventCount >= 200)
    {
        eventCount = 0;
        if (showPageID == PAGE_ID_TOP)
        {
            setLCDpos(1, 1);
            sendSpaceLCD(4);
            setLCDpos(1, 1);
            // sendNum(voltageMonitor::get());
            volt = (uint8_t)(voltageMonitor::get() / 1000);
            sendNum(volt);
            writeLCDdata('.');
            volt = (uint8_t)((voltageMonitor::get() / 100) % 10);
            sendNum(volt);

            // setLCDpos(1, 10);
            //  sendSpaceLCD(4);
            // setLCDpos(1, 10);
            //  sendNum(currentMonitor::getCurrent());

            currTmp = currentMonitor::getCurrent();
            setLCDpos(1, 9);

            if (currTmp >= 10000)
            {
                writeLCDsingleNum(currTmp / 10000);
                currTmp %= 10000;
            }
            else
            {
                sendSpaceLCD(1);
            }

            writeLCDsingleNum(currTmp / 1000);
            currTmp %= 1000;

            setLCDpos(1, 12);
            writeLCDsingleNum(currTmp / 100);
            currTmp %= 100;

            writeLCDsingleNum(currTmp / 10);
        }
    }
}

void throttleAppMain::task()
{
    uint8_t keyCode;

    if (showFlag)
    {
        showFlag = false;
        dispInit();
        /*
        setLCDpos(0, 0);
        writeLCDdata('H');
        writeLCDdata('e');
        writeLCDdata('l');
        writeLCDdata('l');
        writeLCDdata('o');
        writeLCDdata(',');
        writeLCDdata(' ');
        writeLCDdata('W');
        writeLCDdata('o');
        writeLCDdata('r');
        writeLCDdata('l');
        writeLCDdata('d');
        writeLCDdata('!');
        */
    }

    if (keypad::checkChange())
    {
        // setLCDpos(1, 8);
        // sendSpaceLCD(4);
        // setLCDpos(1, 8);
        // sendNum(keypad::readCode());
        /*
        switch (keypad::readCode())
        {
        case KEYCODE_FUNC_A:
            break;
        case KEYCODE_FUNC_B:
            break;
        }
        */

        keyCode = keypad::readCode();
        if (keyCode == KEYCODE_FUNC_A)
        {
            if (showPageID == PAGE_ID_TRAIN_0)
            {
                trainCtrlObj[0].dispTerminate();
                dispInit();
            }
            else
            {
                trainCtrlObj[0].dispInit();
                // keypadCb = trainCtrlObj[0].keypadEvent;
                keypadCb = trainCtrl0KeypadEvent;
                dispEventCb = trainCtrl0DispEvent;
                showPageID = PAGE_ID_TRAIN_0;
            }
        }
        else if (keyCode == KEYCODE_FUNC_B)
        {
            if (showPageID == PAGE_ID_TRAIN_1)
            {
                trainCtrlObj[1].dispTerminate();
                dispInit();
            }
            else
            {
                trainCtrlObj[1].dispInit();
                // keypadCb = trainCtrlObj[1].keypadEvent;
                keypadCb = trainCtrl1KeypadEvent;
                dispEventCb = trainCtrl1DispEvent;
                showPageID = PAGE_ID_TRAIN_1;
            }
        }
        else if (keyCode == KEYCODE_FUNC_SWITCH)
        {
            if (showPageID == PAGE_ID_SWITCH)
            {
                throttleAppSwitchCtrl::dispTerminate();
                dispInit();
            }
            else
            {
                throttleAppSwitchCtrl::dispInit();
                keypadCb = throttleAppSwitchCtrl::keypadEvent;
                dispEventCb = throttleAppSwitchCtrl::dispEvent;
                showPageID = PAGE_ID_SWITCH;
            }
        }
        else if (keyCode == KEYCODE_FUNC_PROG)
        {
            if (showPageID == PAGE_ID_PROG)
            {
                throttleAppProgUI::dispTerminate();
                dispInit();
            }
            else
            {
                throttleAppProgUI::dispInit();
                keypadCb = throttleAppProgUI::keypadEvent;
                dispEventCb = throttleAppProgUI::dispEvent;
                showPageID = PAGE_ID_PROG;
            }
        }
        else if (keypadCb != nullptr)
        {
            keypadCb(keyCode);
        }
    }

    if (directionSW::checkChange(0))
    {
        trainCtrlObj[0].directionEvent(directionSW::readStat(0));
    }

    if (directionSW::checkChange(1))
    {
        trainCtrlObj[1].directionEvent(directionSW::readStat(1));
    }

    trainCtrlObj[0].task();
    trainCtrlObj[1].task();
}

void throttleAppMain::dispInit()
{
    eventRoute = nullptr;
    keypadCb = nullptr;
    dispEventCb = nullptr;

    showPageID = PAGE_ID_TOP;
    clearLCD();
    setLCDpos(0, 0);
    writeLCDdata(trainCtrlObj[0].getPrefixCode());
    writeLCDdata(':');

    if (trainCtrlObj[0].getTrainAddr() < 0xFFFF)
    {
        setLCDpos(0, 2);
        sendNum(trainCtrlObj[0].getTrainAddr());
    }

    setLCDpos(0, 8);
    writeLCDdata(trainCtrlObj[1].getPrefixCode());
    writeLCDdata(':');

    if (trainCtrlObj[1].getTrainAddr() < 0xFFFF)
    {
        setLCDpos(0, 10);
        sendNum(trainCtrlObj[1].getTrainAddr());
    }

    setLCDpos(1, 5);
    writeLCDdata('V');

    // setLCDpos(1, 14);
    // writeLCDdata('m');
    // writeLCDdata('A');
    setLCDpos(1, 11);
    writeLCDdata('.');
    setLCDpos(1, 15);
    writeLCDdata('A');
}

void throttleAppMain::dispEvent()
{
    if (dispEventCb != nullptr)
    {
        dispEventCb();
        return;
    }
}

void throttleAppMain::dispTerminate()
{
}

void throttleAppMain::trainCtrl0KeypadEvent(uint8_t keycode)
{
    trainCtrlObj[0].keypadEvent(keycode);
}

void throttleAppMain::trainCtrl1KeypadEvent(uint8_t keycode)
{
    trainCtrlObj[1].keypadEvent(keycode);
}

void throttleAppMain::trainCtrl0DispEvent()
{
    trainCtrlObj[0].dispEvent();
}

void throttleAppMain::trainCtrl1DispEvent()
{
    trainCtrlObj[1].dispEvent();
}
