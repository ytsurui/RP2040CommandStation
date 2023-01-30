
#include "pico/stdio.h"
#include "th_switchctrl.h"

#include "../acc_ctrl.h"

#include "../../throttle/peripheral/lcd.h"
#include "../../throttle/peripheral/keypad.h"

uint16_t throttleAppSwitchCtrl::selectSWaddr;
bool throttleAppSwitchCtrl::inputFlag;

void throttleAppSwitchCtrl::init(void)
{
    selectSWaddr == 0;
}

void throttleAppSwitchCtrl::event(void)
{
}

void throttleAppSwitchCtrl::task(void)
{
}

void throttleAppSwitchCtrl::dispInit(void)
{
    clearLCD();
    setLCDpos(0, 0);
    writeLCDstring((char *)"Addr:");
    setLCDpos(0, 10);
    writeLCDstring((char *)"Stat:");

    setLCDpos(1, 0);
    writeLCDstring((char *)"C:* T:#  Back:SW");

    if (selectSWaddr != 0)
    {
        setLCDpos(0, 5);
        sendNum(selectSWaddr);
        if (accessoryCtrl::getAccessoryStat(selectSWaddr - 1))
        {
            setLCDpos(0, 15);
            writeLCDdata('C');
        }
        else
        {
            setLCDpos(0, 15);
            writeLCDdata('T');
        }
    }

    setLCDpos(0, 5);
    showCursor();

    inputFlag = false;
}

void throttleAppSwitchCtrl::dispEvent(void)
{
}

void throttleAppSwitchCtrl::dispTerminate(void)
{
    hideCursor();
}

void throttleAppSwitchCtrl::keypadEvent(uint8_t code)
{
    switch (code)
    {
    case KEYCODE_NUM_0:
    case KEYCODE_NUM_1:
    case KEYCODE_NUM_2:
    case KEYCODE_NUM_3:
    case KEYCODE_NUM_4:
    case KEYCODE_NUM_5:
    case KEYCODE_NUM_6:
    case KEYCODE_NUM_7:
    case KEYCODE_NUM_8:
    case KEYCODE_NUM_9:
        if (inputFlag)
        {
            if (selectSWaddr > 204)
            {
                return;
            }
            else if (selectSWaddr == 204)
            {
                if (code == KEYCODE_NUM_9)
                    return;
            }
            selectSWaddr = (selectSWaddr * 10) + code;

            if (selectSWaddr >= 205)
            {
                hideCursor();
            }
        }
        else
        {
            if (code == KEYCODE_NUM_0)
            {
                return;
            }
            selectSWaddr = code;
            inputFlag = true;
        }

        setLCDpos(0, 15);
        if (accessoryCtrl::getAccessoryStat(selectSWaddr - 1))
        {
            writeLCDdata('C');
        }
        else
        {
            writeLCDdata('T');
        }

        setLCDpos(0, 5);
        sendSpaceLCD(4);
        setLCDpos(0, 5);
        sendNum(selectSWaddr);
        break;
    case KEYCODE_FUNC_ASTERISK:
        // change switch

        /*
        setLCDpos(0, 15);
        if (accessoryCtrl::getAccessoryStat(selectSWaddr - 1))
        {
            accessoryCtrl::sendAccessoryPacket((selectSWaddr - 1), false);
            writeLCDdata('T');
        }
        else
        {
            accessoryCtrl::sendAccessoryPacket((selectSWaddr - 1), true);
            writeLCDdata('C');
        }
        */
        if (selectSWaddr == 0)
            break;

        setLCDpos(0, 15);
        accessoryCtrl::sendAccessoryPacket((selectSWaddr - 1), true);
        writeLCDdata('C');

        inputFlag = false;
        setLCDpos(0, 5);
        showCursor();
        break;
    case KEYCODE_FUNC_SHARP:
        // change switch thrown

        if (selectSWaddr == 0)
            break;

        setLCDpos(0, 15);
        accessoryCtrl::sendAccessoryPacket((selectSWaddr - 1), false);
        writeLCDdata('T');

        inputFlag = false;
        setLCDpos(0, 5);
        showCursor();
        break;
    }
}