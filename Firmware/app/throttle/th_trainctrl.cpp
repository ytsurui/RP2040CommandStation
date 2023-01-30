
#include "pico/stdlib.h"
#include "th_trainctrl.h"

#include "../trainctrl.h"

#include "../../throttle/peripheral/lcd.h"
#include "../../throttle/peripheral/keypad.h"

#define FUNC_STAT_SHOW_ROW_POS 4

void throttleAppTrainCtrl::init(char showPrefixCode)
{
    prefixCode = showPrefixCode;
    trainAddr = TRAINCTRL_TRAINADDR_NONSELECT;
    selectingTrainAddr = TRAINCTRL_TRAINADDR_NONSELECT;
    spdCache = 0;
    ctrlFuncGroup = 0;
}

void throttleAppTrainCtrl::event()
{
    lastCtrlEventCount++;
    if (lastCtrlEventCount >= 60000)
    {
        trainctrl::trainctrlresp trainObj;
        trainObj = trainctrl::getTrainCtrl(trainAddr);
        if (trainObj.enable)
        {
            trainObj.train->setSpeed128(directionCache, spdCache);
        }
        lastCtrlEventCount = 0;
    }
}

void throttleAppTrainCtrl::task()
{
}

void throttleAppTrainCtrl::dispInit()
{
    clearLCD();
    setLCDpos(0, 0);
    writeLCDdata(getPrefixCode());
    writeLCDdata(':');

    hideCursor();

    showPage = TRAINCTRL_SHOWPAGE_TOP;

    if (trainAddr == 0xFFFF)
    {
        // Non Selected
        setLCDpos(1, 0);
        writeLCDstring((char *)"Select:*  Back:");
        writeLCDdata(getPrefixCode());
    }
    else
    {
        setLCDpos(0, 3);
        sendNum(trainAddr);
        setLCDpos(0, 9);
        writeLCDstring((char *)"Spd:");

        printSpd = 0xFF;
        printDirection = 0xFF;

        setLCDpos(1, 0);
        printFuncLabel();
        printFuncStat();
    }
}

void throttleAppTrainCtrl::dispEvent()
{
    if (showPage == TRAINCTRL_SHOWPAGE_TOP)
    {
        if (printSpd != spdCache || printDirection != directionCache)
        {
            printSpd = spdCache;
            printDirection = directionCache;

            if (trainAddr != 0xFFFF)
            {
                setLCDpos(0, 13);
                sendSpaceLCD(3);
                setLCDpos(0, 13);
                sendNum(printSpd);
            }
        }
    }
}

void throttleAppTrainCtrl::dispTerminate()
{
}

uint16_t throttleAppTrainCtrl::getTrainAddr(void)
{
    return trainAddr;
}

char throttleAppTrainCtrl::getPrefixCode(void)
{
    return prefixCode;
}

void throttleAppTrainCtrl::keypadEvent(uint8_t code)
{
    switch (code)
    {
    case KEYCODE_FUNC_ASTERISK:
        if (showPage == TRAINCTRL_SHOWPAGE_TOP)
        {
            if (trainAddr == TRAINCTRL_TRAINADDR_NONSELECT)
            {
                showPage = TRAINCTRL_SHOWPAGE_SELECT;
                selectingTrainAddr = TRAINCTRL_TRAINADDR_NONSELECT;
                clearLCD();
                setLCDpos(0, 0);
                writeLCDstring((char *)"Addr: ");
                setLCDpos(1, 0);
                writeLCDstring((char *)"Set:* Cancel:#");

                setLCDpos(0, 6);
                showCursor();
            }
            else
            {
                showPage = TRAINCTRL_SHOWPAGE_RELEASE;
                clearLCD();
                setLCDpos(0, 0);
                writeLCDstring((char *)"Addr: ");
                sendNum(trainAddr);
                setLCDpos(1, 0);
                writeLCDstring((char *)"Release:* Back:#");

                hideCursor();
            }
        }
        else if (showPage == TRAINCTRL_SHOWPAGE_SELECT)
        {
            if (selectingTrainAddr != TRAINCTRL_TRAINADDR_NONSELECT)
            {
                trainctrl::trainctrlresp respData;

                respData = trainctrl::selectNewTrain(selectingTrainAddr);

                if (!respData.enable)
                {
                    // Error
                    return;
                }

                trainAddr = selectingTrainAddr;
                ctrlFuncGroup = 0;
                selectingTrainAddr = TRAINCTRL_TRAINADDR_NONSELECT;
                dispInit();
                return;
            }
        }
        else if (showPage == TRAINCTRL_SHOWPAGE_RELEASE)
        {
            trainctrl::releaseTrain(trainAddr);
            trainAddr = TRAINCTRL_TRAINADDR_NONSELECT;
            spdCache = 0;
            dispInit();
            return;
        }
        break;

    case KEYCODE_FUNC_SHARP:
        switch (showPage)
        {
        case TRAINCTRL_SHOWPAGE_SELECT:
            dispInit();
            break;
        case TRAINCTRL_SHOWPAGE_RELEASE:
            dispInit();
            break;
        }

        break;
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
        if (showPage == TRAINCTRL_SHOWPAGE_TOP)
        {
            if (trainAddr == TRAINCTRL_TRAINADDR_NONSELECT)
                return;

            trainctrl::trainctrlresp trainObj;
            uint8_t nowFunc;

            trainObj = trainctrl::getTrainCtrl(trainAddr);

            // Function Control
            switch (ctrlFuncGroup)
            {
            case 0:
                // F0-F9
                if (code == 0)
                {
                    // F0
                    nowFunc = trainObj.train->getFuncG1();
                    nowFunc ^= 0x10;
                    trainObj.train->setFuncG1(nowFunc);
                    lastCtrlEventCount = 0;

                    setLCDpos(1, FUNC_STAT_SHOW_ROW_POS);
                    printFuncMark(nowFunc & 0x10);
                }
                else if (code <= 4)
                {
                    // F1-F4
                    nowFunc = trainObj.train->getFuncG1();
                    nowFunc ^= (0x01 << (code - 1));
                    trainObj.train->setFuncG1(nowFunc);
                    lastCtrlEventCount = 0;

                    setLCDpos(1, (FUNC_STAT_SHOW_ROW_POS + code));
                    printFuncMark(nowFunc & (0x01 << (code - 1)));
                }
                else if (code <= 8)
                {
                    // F5-F8
                    nowFunc = trainObj.train->getFuncG2();
                    nowFunc ^= (0x01 << (code - 5));
                    trainObj.train->setFuncG2(nowFunc);
                    lastCtrlEventCount = 0;

                    setLCDpos(1, (FUNC_STAT_SHOW_ROW_POS + code));
                    printFuncMark(nowFunc & (0x01 << (code - 5)));
                }
                else
                {
                    // F9
                    nowFunc = trainObj.train->getFuncG3();
                    nowFunc ^= 0x01;
                    trainObj.train->setFuncG3(nowFunc);
                    lastCtrlEventCount = 0;

                    setLCDpos(1, (FUNC_STAT_SHOW_ROW_POS + code));
                    printFuncMark(nowFunc & 0x01);
                }

                break;
            case 1:
                // F10-F19
                if (code <= 2)
                {
                    // F10-F12
                    nowFunc = trainObj.train->getFuncG3();
                    nowFunc ^= (0x02 << code);
                    trainObj.train->setFuncG3(nowFunc);
                    lastCtrlEventCount = 0;

                    setLCDpos(1, (FUNC_STAT_SHOW_ROW_POS + code));
                    printFuncMark(nowFunc & (0x02 << code));
                }
                else
                {
                    // F13-F19
                    nowFunc = trainObj.train->getFuncG4();
                    nowFunc ^= (0x01 << (code - 3));
                    trainObj.train->setFuncG4(nowFunc);
                    lastCtrlEventCount = 0;

                    setLCDpos(1, (FUNC_STAT_SHOW_ROW_POS + code));
                    printFuncMark(nowFunc & (0x01 << (code - 3)));
                }
                break;
            case 2:
                // F20-F28
                if (code == 0)
                {
                    // F20
                    nowFunc = trainObj.train->getFuncG4();
                    nowFunc ^= 0x80;
                    trainObj.train->setFuncG4(nowFunc);
                    lastCtrlEventCount = 0;

                    setLCDpos(1, FUNC_STAT_SHOW_ROW_POS);
                    printFuncMark(nowFunc & 0x80);
                }
                else if (code <= 8)
                {
                    // F21-F28
                    nowFunc = trainObj.train->getFuncG5();
                    nowFunc ^= (0x01 << (code - 1));
                    trainObj.train->setFuncG5(nowFunc);
                    lastCtrlEventCount = 0;

                    setLCDpos(1, (FUNC_STAT_SHOW_ROW_POS + code));
                    printFuncMark(nowFunc & (0x01 << (code - 1)));
                }
                break;
            }
        }
        else if (showPage == TRAINCTRL_SHOWPAGE_SELECT)
        {
            if (selectingTrainAddr == TRAINCTRL_TRAINADDR_NONSELECT)
            {
                selectingTrainAddr = code;
            }
            else
            {
                if (selectingTrainAddr < 1000)
                {
                    selectingTrainAddr = selectingTrainAddr * 10;
                    selectingTrainAddr += code;
                    if (selectingTrainAddr >= 1000)
                    {
                        hideCursor();
                    }
                }
            }
            setLCDpos(0, 6);
            sendNum(selectingTrainAddr);
        }
        break;
    case (KEYCODE_NUM_1 + KEYCODE_WITH_SHARP):
    case (KEYCODE_NUM_2 + KEYCODE_WITH_SHARP):
    case (KEYCODE_NUM_3 + KEYCODE_WITH_SHARP):
        // Change Func Group Operation
        ctrlFuncGroup = (code - KEYCODE_WITH_SHARP - 1);
        setLCDpos(1, 0);
        printFuncLabel();
        printFuncStat();
        break;
    }
}

void throttleAppTrainCtrl::directionEvent(uint8_t dir)
{
    directionCache = dir;

    trainctrl::trainctrlresp trainObj;

    trainObj = trainctrl::getTrainCtrl(trainAddr);
    if (trainObj.enable)
    {
        trainObj.train->setSpeed128(directionCache, spdCache);
    }

    lastCtrlEventCount = 0;
}

void throttleAppTrainCtrl::volumeEvent(uint8_t value)
{
    if (trainAddr == TRAINCTRL_TRAINADDR_NONSELECT)
        return;

    if ((value == spdCache) && (oldSendDirection == directionCache))
    {
        return;
    }

    spdCache = value;
    oldSendDirection = directionCache;

    trainctrl::trainctrlresp trainObj;

    trainObj = trainctrl::getTrainCtrl(trainAddr);
    trainObj.train->setSpeed128(directionCache, value);

    lastCtrlEventCount = 0;
    /*
    setLCDpos(0, 13);
    sendSpaceLCD(3);
    setLCDpos(0, 13);
    sendNum(value);
    */
}

void throttleAppTrainCtrl::printFuncMark(bool stat)
{
    if (stat)
    {
        writeLCDdata('*');
    }
    else
    {
        writeLCDdata('-');
    }
}

void throttleAppTrainCtrl::printFuncLabel(void)
{
    writeLCDdata('F');
    writeLCDdata(0x30 + ctrlFuncGroup);
    writeLCDdata('0');
    writeLCDdata(':');
}

void throttleAppTrainCtrl::printFuncStat(void)
{

    if (trainAddr == TRAINCTRL_TRAINADDR_NONSELECT)
        return;

    uint8_t funcStat, i;
    trainctrl::trainctrlresp trainObj;

    trainObj = trainctrl::getTrainCtrl(trainAddr);

    setLCDpos(1, FUNC_STAT_SHOW_ROW_POS);
    switch (ctrlFuncGroup)
    {
    case 0:
        // F0-F9
        funcStat = trainObj.train->getFuncG1();
        printFuncMark(funcStat & 0x10);
        for (i = 0; i < 4; i++)
        {
            printFuncMark(funcStat & (0x01 << i));
        }

        funcStat = trainObj.train->getFuncG2();
        for (i = 0; i < 4; i++)
        {
            printFuncMark(funcStat & (0x01 << i));
        }

        funcStat = trainObj.train->getFuncG3();
        printFuncMark(funcStat & 0x01);
        break;
    case 1:
        // F10-F19
        funcStat = trainObj.train->getFuncG3();
        for (i = 0; i < 3; i++)
        {
            printFuncMark(funcStat & (0x02 << i));
        }
        funcStat = trainObj.train->getFuncG4();
        for (i = 0; i < 7; i++)
        {
            printFuncMark(funcStat & (0x01 << i));
        }
        break;
    case 2:
        // F20-F28
        funcStat = trainObj.train->getFuncG4();
        printFuncMark(funcStat & 0x80);
        funcStat = trainObj.train->getFuncG5();
        for (i = 0; i < 8; i++)
        {
            printFuncMark(funcStat & (0x01 << i));
        }
        break;
    }
}