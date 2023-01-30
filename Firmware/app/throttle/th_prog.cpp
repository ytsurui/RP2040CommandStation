
#include "pico/stdlib.h"
#include "th_prog.h"

#include "../progtrak.h"

#include "../../throttle/peripheral/lcd.h"
#include "../../throttle/peripheral/keypad.h"

uint16_t throttleAppProgUI::cvNum;
uint8_t throttleAppProgUI::cvValue;
uint16_t throttleAppProgUI::progAddr;
bool throttleAppProgUI::inputFlag, throttleAppProgUI::inputFlag2;
uint8_t throttleAppProgUI::mode;

uint8_t throttleAppProgUI::progressOldValue, throttleAppProgUI::progressOldTotal;

uint8_t throttleAppProgUI::readAddrCV17temp;

#define PROGMODE_SELECT_MODE 0x10
#define PROGMODE_ADDR_TOP 0x04
#define PROGMODE_INPUT_ADDR 0x05
#define PROGMODE_READING_ADDR 0x44
#define PROGMODE_WRITING_ADDR 0x64

#define PROGMODE_INPUT_CV 0x01
#define PROGMODE_INPUT_VALUE 0x02
#define PROGMODE_READING 0x40
#define PROGMODE_WRITING 0x60

void throttleAppProgUI::init(void)
{
}

void throttleAppProgUI::event(void)
{
}

void throttleAppProgUI::task(void)
{
}

void throttleAppProgUI::dispInit(void)
{
    clearLCD();
    setLCDpos(0, 0);
    writeLCDstring((char *)"Addr Program:  1");
    setLCDpos(1, 0);
    writeLCDstring((char *)"CV Programmer: 2");
    hideCursor();

    inputFlag = false;
    inputFlag2 = false;
    mode = PROGMODE_SELECT_MODE;
}

void throttleAppProgUI::dispCVprogrammerInit(void)
{
    clearLCD();
    setLCDpos(0, 0);
    writeLCDstring((char *)"CV:");
    setLCDpos(0, 8);
    writeLCDstring((char *)"Val:");

    setLCDpos(1, 0);
    writeLCDstring((char *)"Read:* Write:#");
    setLCDpos(0, 3);
    showCursor();

    inputFlag = false;
    inputFlag2 = false;
    cvNum = 0;
    cvValue = 0;
    mode = PROGMODE_INPUT_CV;
}

void throttleAppProgUI::dispAddrProgrammerInit(void)
{
    clearLCD();
    setLCDpos(0, 0);
    writeLCDstring((char *)"Addr:");
    setLCDpos(1, 0);
    writeLCDstring((char *)"Read:* Write:#");
    setLCDpos(0, 5);
    showCursor();

    inputFlag = false;
    inputFlag2 = false;
    progAddr = 0xFFFF;
    mode = PROGMODE_INPUT_ADDR;
}

void throttleAppProgUI::dispEvent(void)
{
}

void throttleAppProgUI::dispTerminate(void)
{

    hideCursor();
}

void throttleAppProgUI::keypadEvent(uint8_t code)
{
    switch (code)
    {
    case KEYCODE_NUM_0:
    case KEYCODE_NUM_1:
        if (mode == PROGMODE_SELECT_MODE)
        {
            dispAddrProgrammerInit();
            break;
        }
    case KEYCODE_NUM_2:
        if (mode == PROGMODE_SELECT_MODE)
        {
            dispCVprogrammerInit();
            break;
        }
    case KEYCODE_NUM_3:
    case KEYCODE_NUM_4:
    case KEYCODE_NUM_5:
    case KEYCODE_NUM_6:
    case KEYCODE_NUM_7:
    case KEYCODE_NUM_8:
    case KEYCODE_NUM_9:
        if (mode == PROGMODE_INPUT_CV)
        {
            if (inputFlag)
            {
                if (cvNum > 102)
                {
                    return;
                }
                else if (cvNum == 102)
                {
                    if (code >= KEYCODE_NUM_5)
                    {
                        return;
                    }
                }

                cvNum = (cvNum * 10) + code;
            }
            else
            {
                setLCDpos(1, 0);
                writeLCDstring((char *)"Read:* Write:#");
                sendSpaceLCD(2);
                setLCDpos(0, 3);
                sendSpaceLCD(4);
                cvNum = code;
                inputFlag = true;
            }

            if (cvNum == 0)
            {
                break;
            }

            setLCDpos(0, 3);
            sendNum(cvNum);

            if (cvNum > 102)
            {
                hideCursor();
            }
        }
        else if (mode == PROGMODE_INPUT_VALUE)
        {
            if (inputFlag)
            {
                if (cvValue > 25)
                {
                    return;
                }
                else if (cvValue == 25)
                {
                    if (code >= KEYCODE_NUM_6)
                    {
                        return;
                    }
                }

                cvValue = (cvValue * 10) + code;
            }
            else
            {
                cvValue = code;
                inputFlag = true;
            }

            setLCDpos(0, 12);
            sendNum(cvValue);

            if (cvNum > 25)
            {
                hideCursor();
            }
        }
        else if (mode == PROGMODE_INPUT_ADDR)
        {
            if (inputFlag)
            {
                if (progAddr > 998)
                {
                    return;
                }
                else if (progAddr == 998)
                {
                    if (code >= KEYCODE_NUM_4)
                    {
                        return;
                    }
                }
                progAddr = (progAddr * 10) + code;
            }
            else
            {
                progAddr = code;
                inputFlag = true;
            }

            setLCDpos(0, 5);
            sendSpaceLCD(4);
            setLCDpos(0, 5);
            sendNum(progAddr);

            if (progAddr > 998)
            {
                hideCursor();
            }
        }
        break;
    case KEYCODE_FUNC_ASTERISK:
        if (mode == PROGMODE_INPUT_CV)
        {
            // Read Mode
            if (cvNum == 0)
            {
                break;
            }

            inputFlag = false;

            // progtrak::setProgMode(PROGMODE_DIRECT_BYTE);
            progtrak::setProgMode(PROGMODE_DIRECT_BIT);
            progtrak::setReadWrite(PROGMODE_READ);
            progtrak::setCVnum(cvNum);
            progtrak::setCallback(readCallBack);
            progtrak::setProgressCallback(readProgressCallBack);
            progtrak::RunProg();
            mode = PROGMODE_READING;

            setLCDpos(1, 0);
            // writeLCDstring((char *)"Reading...");
            writeLCDstring((char *)"Scanning:");
            sendSpaceLCD(3);
            writeLCDdata('/');
            sendSpaceLCD(3);
            hideCursor();
            progressOldValue = 0xFF;
            progressOldTotal = 0;

            // setLCDpos(1, 10);
            // sendSpaceLCD(6);
        }
        else if (mode == PROGMODE_INPUT_VALUE)
        {
            // Back To CV num input mode
            inputFlag = false;
            mode = PROGMODE_INPUT_CV;
            setLCDpos(1, 0);
            writeLCDstring((char *)"Read:* Write:#");
            sendSpaceLCD(2);

            setLCDpos(0, 12);
            sendSpaceLCD(4);

            setLCDpos(0, 3);
            showCursor();
        }
        else if (mode == PROGMODE_INPUT_ADDR)
        {
            // Read Train Addr
            setLCDpos(0, 5);
            sendSpaceLCD(4);
            inputFlag = false;
            progtrak::setProgMode(PROGMODE_DIRECT_BIT);
            progtrak::setReadWrite(PROGMODE_READ);
            progtrak::setCVnum(29);
            progtrak::setCallback(readAddrCV29Callback);
            progtrak::setProgressCallback(readProgressCallBack);
            progtrak::RunProg();
            mode = PROGMODE_READING_ADDR;

            setLCDpos(1, 0);
            writeLCDstring((char *)"Get CV29:");
            sendSpaceLCD(3);
            writeLCDdata('/');
            sendSpaceLCD(3);
            hideCursor();
            progressOldValue = 0xFF;
            progressOldTotal = 0;
        }
        break;
    case KEYCODE_FUNC_SHARP:
        if (mode == PROGMODE_INPUT_CV)
        {
            // Input CV value mode
            inputFlag = false;
            if (cvNum == 0)
            {
                break;
            }

            mode = PROGMODE_INPUT_VALUE;
            setLCDpos(1, 0);
            writeLCDstring((char *)"Back:* Write:#");
            sendSpaceLCD(2);
            setLCDpos(0, 12);
            showCursor();
        }
        else if (mode == PROGMODE_INPUT_VALUE)
        {
            // Write Mode
            if (cvNum == 0)
            {
                break;
            }
            if (inputFlag == false)
            {
                break;
            }
            inputFlag = false;

            progtrak::setProgMode(PROGMODE_DIRECT_BYTE);
            progtrak::setReadWrite(PROGMODE_WRITE);
            progtrak::setCVnum(cvNum);
            progtrak::setCVvalue(cvValue);
            progtrak::setCallback(writeCallBack);
            progtrak::setProgressCallback(nullptr);
            progtrak::RunProg();
            mode = PROGMODE_WRITING;

            setLCDpos(1, 0);
            writeLCDstring((char *)"Write...");
            sendSpaceLCD(8);
            hideCursor();
        }
        else if (mode == PROGMODE_INPUT_ADDR)
        {
            // Write Address Mode
            if (progAddr == 0 || progAddr == 0xFFFF)
            {
                break;
            }
            inputFlag = false;

            progtrak::setProgMode(PROGMODE_DIRECT_BIT);
            progtrak::setReadWrite(PROGMODE_WRITE);
            progtrak::setCVnum(29);
            progtrak::setCVbitPos(5);
            if (progAddr > 127)
            {
                // 14-bit Addr
                progtrak::setCVbitValue(true);
            }
            else
            {
                // 7-bit Addr
                progtrak::setCVbitValue(false);
            }
            progtrak::setCallback(writeAddrCV29Callback);
            progtrak::setProgressCallback(nullptr);
            progtrak::RunProg();
            mode = PROGMODE_WRITING_ADDR;

            setLCDpos(1, 0);
            writeLCDstring((char *)"Write CV29...");
            sendSpaceLCD(3);
            hideCursor();
        }
        break;
    }
}

void throttleAppProgUI::readCallBack(uint8_t cvValue, bool stat)
{
    if (mode == PROGMODE_READING)
    {
        if (stat)
        {
            setLCDpos(1, 0);
            sendSpaceLCD(16);
            setLCDpos(1, 0);
            writeLCDstring((char *)"Read Value:");
            setLCDpos(1, 12);
            sendSpaceLCD(4);
            setLCDpos(1, 12);
            sendNum(cvValue);
            mode = PROGMODE_INPUT_CV;

            setLCDpos(0, 3);
            showCursor();
        }
        else
        {
            setLCDpos(1, 0);
            sendSpaceLCD(16);
            setLCDpos(1, 0);
            writeLCDstring((char *)"Read Error");
            mode = PROGMODE_INPUT_CV;

            setLCDpos(0, 3);
            showCursor();
        }
    }
    else if (mode == PROGMODE_READING_ADDR)
    {
    }
}

void throttleAppProgUI::readProgressCallBack(uint8_t nowValue, uint8_t total)
{
    if (mode == PROGMODE_READING || mode == PROGMODE_READING_ADDR)
    {

        if (progressOldValue != nowValue)
        {
            setLCDpos(1, 9);
            sendSpaceLCD(3);
            setLCDpos(1, 9);
            sendNum(nowValue);
            progressOldValue = nowValue;
        }

        if (progressOldTotal != total)
        {
            setLCDpos(1, 13);
            sendSpaceLCD(3);
            setLCDpos(1, 13);
            sendNum(total);
            progressOldTotal = total;
        }
    }
}

void throttleAppProgUI::writeCallBack(uint8_t cvValue, bool stat)
{
    if (mode == PROGMODE_WRITING)
    {
        if (stat)
        {
            setLCDpos(1, 0);
            writeLCDstring((char *)"Write Complete");
            sendSpaceLCD(2);

            mode = PROGMODE_INPUT_CV;
            setLCDpos(0, 3);
            showCursor();
        }
        else
        {
            setLCDpos(1, 0);
            writeLCDstring((char *)"Write Error");
            sendSpaceLCD(5);

            mode = PROGMODE_INPUT_CV;
            setLCDpos(0, 3);
            showCursor();
        }
    }
}

void throttleAppProgUI::readAddrCV29Callback(uint8_t cvValue, bool stat)
{
    if (mode == PROGMODE_READING_ADDR)
    {
        if (stat)
        {
            // Read CV29 Success
            if (cvValue & 0x20)
            {
                // 14-bit Addr (CV17 & CV18)
                inputFlag = false;
                progtrak::setProgMode(PROGMODE_DIRECT_BIT);
                progtrak::setReadWrite(PROGMODE_READ);
                progtrak::setCVnum(17);
                progtrak::setCallback(readAddrCV17Callback);
                progtrak::setProgressCallback(readProgressCallBack);
                progtrak::RunProg();

                setLCDpos(1, 0);
                writeLCDstring((char *)"Get CV17:");
                sendSpaceLCD(3);
                writeLCDdata('/');
                sendSpaceLCD(3);
                hideCursor();
                progressOldValue = 0xFF;
                progressOldTotal = 0;
            }
            else
            {
                // 7-bit Addr (CV1)
                inputFlag = false;
                progtrak::setProgMode(PROGMODE_DIRECT_BIT);
                progtrak::setReadWrite(PROGMODE_READ);
                progtrak::setCVnum(1);
                progtrak::setCallback(readAddrCV1Callback);
                progtrak::setProgressCallback(readProgressCallBack);
                progtrak::RunProg();

                setLCDpos(1, 0);
                writeLCDstring((char *)"Get CV1: ");
                sendSpaceLCD(3);
                writeLCDdata('/');
                sendSpaceLCD(3);
                hideCursor();
                progressOldValue = 0xFF;
                progressOldTotal = 0;
            }
        }
        else
        {
            setLCDpos(1, 0);
            sendSpaceLCD(16);
            setLCDpos(1, 0);
            writeLCDstring((char *)"Read Error");
            mode = PROGMODE_INPUT_ADDR;

            setLCDpos(0, 5);
            showCursor();
        }
    }
}

void throttleAppProgUI::readAddrCV1Callback(uint8_t cvValue, bool stat)
{
    if (mode == PROGMODE_READING_ADDR)
    {
        if (stat)
        {
            setLCDpos(1, 0);
            writeLCDstring((char *)"Read Complete");
            sendSpaceLCD(3);

            progAddr = cvValue;
            setLCDpos(0, 5);
            sendNum(progAddr);
            mode = PROGMODE_INPUT_ADDR;
            setLCDpos(0, 5);
            showCursor();
        }
        else
        {
            setLCDpos(1, 0);
            sendSpaceLCD(16);
            setLCDpos(1, 0);
            writeLCDstring((char *)"Read Error");
            mode = PROGMODE_INPUT_ADDR;

            setLCDpos(0, 5);
            showCursor();
        }
    }
}

void throttleAppProgUI::readAddrCV17Callback(uint8_t cvValue, bool stat)
{
    if (mode == PROGMODE_READING_ADDR)
    {
        if (stat)
        {
            inputFlag = false;
            progtrak::setProgMode(PROGMODE_DIRECT_BIT);
            progtrak::setReadWrite(PROGMODE_READ);
            progtrak::setCVnum(18);
            progtrak::setCallback(readAddrCV18Callback);
            progtrak::setProgressCallback(readProgressCallBack);
            progtrak::RunProg();

            readAddrCV17temp = cvValue;

            setLCDpos(1, 0);
            writeLCDstring((char *)"Get CV18:");
            sendSpaceLCD(3);
            writeLCDdata('/');
            sendSpaceLCD(3);
            hideCursor();
            progressOldValue = 0xFF;
            progressOldTotal = 0;
        }
        else
        {
            setLCDpos(1, 0);
            sendSpaceLCD(16);
            setLCDpos(1, 0);
            writeLCDstring((char *)"Read Error");
            mode = PROGMODE_INPUT_ADDR;

            setLCDpos(0, 5);
            showCursor();
        }
    }
}

void throttleAppProgUI::readAddrCV18Callback(uint8_t cvValue, bool stat)
{
    if (mode == PROGMODE_READING_ADDR)
    {
        if (stat)
        {
            setLCDpos(1, 0);
            writeLCDstring((char *)"Read Complete");
            sendSpaceLCD(3);

            progAddr = (uint16_t)((readAddrCV17temp & 0x3F) << 8) + (uint16_t)cvValue;

            setLCDpos(0, 5);
            sendNum(progAddr);
            mode = PROGMODE_INPUT_ADDR;
            setLCDpos(0, 5);
            showCursor();
        }
        else
        {
            setLCDpos(1, 0);
            sendSpaceLCD(16);
            setLCDpos(1, 0);
            writeLCDstring((char *)"Read Error");
            mode = PROGMODE_INPUT_ADDR;

            setLCDpos(0, 5);
            showCursor();
        }
    }
}

void throttleAppProgUI::writeAddrCV29Callback(uint8_t cvValue, bool stat)
{
    if (mode == PROGMODE_WRITING_ADDR)
    {
        // if (stat)
        //{
        progtrak::setProgMode(PROGMODE_DIRECT_BYTE);
        progtrak::setReadWrite(PROGMODE_WRITE);
        if (progAddr > 127)
        {
            // 14-bit Addr
            progtrak::setCVnum(17);
            progtrak::setCVvalue(0xC0 | ((uint8_t)((progAddr >> 8) & 0x3F)));
            progtrak::setCallback(writeAddrCV17Callback);

            setLCDpos(1, 0);
            writeLCDstring((char *)"Write CV17...");
        }
        else
        {
            // 7-bit Addr
            progtrak::setCVnum(1);
            progtrak::setCVvalue((uint8_t)progAddr);
            progtrak::setCallback(writeAddrCV1Callback);

            setLCDpos(1, 0);
            writeLCDstring((char *)"Write CV1... ");
        }
        progtrak::setProgressCallback(nullptr);
        progtrak::RunProg();
        //}
        // else
        //{
        //    setLCDpos(1, 0);
        //    sendSpaceLCD(16);
        //    setLCDpos(1, 0);
        //    writeLCDstring((char *)"Write Error");
        //    mode = PROGMODE_INPUT_ADDR;
        //
        //    setLCDpos(0, 5);
        //    showCursor();
        //}
    }
}

void throttleAppProgUI::writeAddrCV1Callback(uint8_t cvValue, bool stat)
{
    if (mode == PROGMODE_WRITING_ADDR)
    {
        // if (stat)
        //{
        setLCDpos(1, 0);
        writeLCDstring((char *)"Write Complete");
        sendSpaceLCD(2);

        mode = PROGMODE_INPUT_ADDR;
        setLCDpos(0, 5);
        showCursor();
        //}
        // else
        //{
        //    setLCDpos(1, 0);
        //    sendSpaceLCD(16);
        //    setLCDpos(1, 0);
        //    writeLCDstring((char *)"Write Error");
        //    mode = PROGMODE_INPUT_ADDR;
        //
        //    setLCDpos(0, 5);
        //    showCursor();
        //}
    }
}

void throttleAppProgUI::writeAddrCV17Callback(uint8_t cvValue, bool stat)
{
    if (mode == PROGMODE_WRITING_ADDR)
    {
        // if (stat)
        //{
        progtrak::setProgMode(PROGMODE_DIRECT_BYTE);
        progtrak::setReadWrite(PROGMODE_WRITE);
        progtrak::setCVnum(18);
        progtrak::setCVvalue((uint8_t)((progAddr & 0x00FF)));
        progtrak::setCallback(writeAddrCV18Callback);

        setLCDpos(1, 0);
        writeLCDstring((char *)"Write CV18...");

        progtrak::setProgressCallback(nullptr);
        progtrak::RunProg();
        //}
        // else
        //{
        //    setLCDpos(1, 0);
        //    sendSpaceLCD(16);
        //    setLCDpos(1, 0);
        //    writeLCDstring((char *)"Write Error");
        //    mode = PROGMODE_INPUT_ADDR;
        //
        //    setLCDpos(0, 5);
        //    showCursor();
        //}
    }
}

void throttleAppProgUI::writeAddrCV18Callback(uint8_t cvValue, bool stat)
{
    if (mode == PROGMODE_WRITING_ADDR)
    {
        // if (stat)
        //{
        setLCDpos(1, 0);
        writeLCDstring((char *)"Write Complete");
        sendSpaceLCD(2);

        mode = PROGMODE_INPUT_ADDR;
        setLCDpos(0, 5);
        showCursor();
        //}
        // else
        //{
        //    setLCDpos(1, 0);
        //    sendSpaceLCD(16);
        //    setLCDpos(1, 0);
        //    writeLCDstring((char *)"Write Error");
        //    mode = PROGMODE_INPUT_ADDR;
        //
        //    setLCDpos(0, 5);
        //    showCursor();
        //}
    }
}