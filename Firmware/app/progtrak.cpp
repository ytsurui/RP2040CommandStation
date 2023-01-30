
#include "pico/stdlib.h"
#include "progtrak.h"
#include "dccsignal.h"

#include "../peripheral/adcReader.h"

#include "../peripheral/dccsignalport.h"

#include <stdio.h>

bool progtrak::progModeFlag = false;
bool progtrak::readCurrentFlag = false;

void progtrak::sendResetPacket(void)
{
    uint8_t packet[3];
    packet[0] = 0;
    packet[1] = 0;

    dccsignal::calcChecksumPacket(packet, 2);
    // return dccsignal::putPacket(packet, 3, 3, 0xFFFF, 0);
    dccport::dcc_send_packet(packet, 3, 3);
}

void progtrak::sendIdlePacket(void)
{
    uint8_t packet[3];
    packet[0] = 0xFF;
    packet[1] = 0;

    dccsignal::calcChecksumPacket(packet, 2);
    // return dccsignal::putPacket(packet, 3, 3, 0xFFFF, 0);
    dccport::dcc_send_packet(packet, 3, 3);
}

bool progtrak::sendOpsModeWriteBit(uint16_t trainAddr, uint16_t cvNum, uint8_t pos, bool data)
{
    return false;
}

bool progtrak::sendOpsModeWriteByte(uint16_t trainAddr, uint16_t cvNum, uint8_t writeValue)
{
    return false;
}

void progtrak::sendDirectReadByte(uint16_t cvNum, uint8_t checkDataValue)
{
    uint8_t packet[4];
    uint8_t i;
    uint16_t readTmp;

    packet[0] = 0x74 | ((cvNum >> 8) & 0x03);
    packet[1] = cvNum & 0x00FF;
    packet[2] = checkDataValue;

    dccsignal::calcChecksumPacket(packet, 3);
    // return dccsignal::putPacket(packet, 3, 3, 0xFFFF, 0);
    // dccport::dcc_send_packet(packet, 4, 1);
    // readCurrentFlag = true;
    // adcCurrentReadEvent();
    // dccport::dcc_send_packet(packet, 4, 5);

    currentStat = false;

    for (i = 0; i < 6; i++)
    {
        dccport::dcc_send_packet(packet, 4, 1);
        /*
        readTmp = adcCurrentReadEvent();

        if (initCurrentValue < readTmp)
        {
            // printf("CV:%d, data=%d, curr=%d\n", cvNum, checkDataValue, readTmp);
            if ((readTmp - initCurrentValue) > 20)
            {
                currentStat = true;
            }
        }
        */
    }
}

void progtrak::sendDirectReadBit(uint16_t cvNum, uint8_t checkBitPos, uint8_t bitValue)
{
    uint8_t packet[4];

    if (checkBitPos > 7)
    {
        return;
    }

    packet[0] = 0x78 | ((cvNum >> 8) & 0x03);
    packet[1] = cvNum & 0x00FF;
    packet[2] = 0xE0 | checkBitPos;

    if (bitValue)
        packet[2] |= 0x08;

    dccsignal::calcChecksumPacket(packet, 3);
    // return dccsignal::putPacket(packet, 3, 3, 0xFFFF, 0);
    dccport::dcc_send_packet(packet, 4, 1);
    dccport::dcc_send_packet(packet, 4, 5);
}

void progtrak::sendDirectWriteByte(uint16_t cvNum, uint8_t writeValue)
{
    uint8_t packet[4];

    packet[0] = 0x7C | ((cvNum >> 8) & 0x03);
    packet[1] = cvNum & 0x00FF;
    packet[2] = writeValue;

    dccsignal::calcChecksumPacket(packet, 3);
    // return dccsignal::putPacket(packet, 3, 3, 0xFFFF, 0);
    dccport::dcc_send_packet(packet, 4, 6);
}

void progtrak::sendDirectWriteBit(uint16_t cvNum, uint8_t pos, bool value)
{
    uint8_t packet[4];

    if (pos > 7)
    {
        return;
    }

    packet[0] = 0x78 | ((cvNum >> 8) & 0x03);
    packet[1] = cvNum & 0x00FF;
    packet[2] = 0xF0 | pos;

    if (value)
        packet[2] |= 0x08;

    dccsignal::calcChecksumPacket(packet, 3);
    dccport::dcc_send_packet(packet, 4, 1);
    dccport::dcc_send_packet(packet, 4, 5);
}

bool progtrak::checkProgMode(void)
{
    return progModeFlag;
}

bool progtrak::readCurrentRunFlag(void)
{
    return readCurrentFlag;
    /*
    if (readCurrentFlag)
    {
        readCurrentFlag = false;
        return true;
    }
    return false;
    */
}

void progtrak::readCurrent(void)
{
    uint16_t readTmp;
    readTmp = adcCurrentReadEvent();

    if (initCurrentValue < readTmp)
    {
        // printf("readCurrent curr=%d\n", readTmp);
        if ((readTmp - initCurrentValue) > 5) // 18mA
        {
            currentStat = true;
        }
    }
}

void progtrak::readDirectModeBit(uint16_t cvNum)
{
    uint8_t i, i2;
    uint8_t readBit;
    uint8_t readValue = 0;
    // bool readStat = false;
    bool err = false;

    dccport::setPowerStat(false);
    sleep_ms(1000);
    dccport::setProgmode(true);
    dccport::setPowerStat(true);

    sendResetPacket();
    sendResetPacket();
    sendResetPacket();
    sendResetPacket();
    sendResetPacket();
    sendResetPacket();
    initCurrentValue = readADCcachedValue(0);
    readCurrentFlag = true;

    for (i = 0; i < 8; i++)
    {
        readBit = 0xFF;
        for (i2 = 0; i2 < 3; i2++)
        {
            if (progProgressCb != nullptr)
            {
                progProgressCb(i + 1, 8);
            }
            sendResetPacket();
            sendResetPacket();
            currentStat = false;
            sendDirectReadBit(cvNum, i, 0);
            if (currentStat)
            {
                // readStat = true;
                readBit = 0;
                break;
            }
            sendResetPacket();
            sendResetPacket();
            currentStat = false;
            sendDirectReadBit(cvNum, i, 1);
            if (currentStat)
            {
                // readStat = true;
                // readValue |= (0x01 << i);
                readBit = 1;
                break;
            }
        }
        switch (readBit)
        {
        case 0:
            break;
        case 1:
            readValue |= (0x01 << i);
            continue;
        default:
            err = true;
            break;
        }
        /*
        if (progProgressCb != nullptr)
        {
            progProgressCb(i + 1, 8);
        }
        sendResetPacket();
        sendResetPacket();
        currentStat = false;
        sendDirectReadBit(cvNum, i, 0);
        if (currentStat)
        {
            // readStat = true;
            continue;
        }
        sendResetPacket();
        sendResetPacket();
        currentStat = false;
        sendDirectReadBit(cvNum, i, 1);
        if (currentStat)
        {
            // readStat = true;
            readValue |= (0x01 << i);
            continue;
        }
        err = true;
        break;
        */

        if (err)
        {
            break;
        }
    }

    if (!err)
    {
        progCVvalue = readValue;
        progStat = true;
    }
    else
    {
        progStat = false;
    }

    readCurrentFlag = false;
    dccport::setPowerStat(false);
    dccport::setProgmode(false);
}

void progtrak::readDirectModeByte(uint16_t cvNum)
{
    uint8_t i;
    // uint16_t readADCtemp;

    dccport::setPowerStat(false);
    sleep_ms(1000);
    dccport::setProgmode(true);
    dccport::setPowerStat(true);

    sendResetPacket();
    sendResetPacket();
    sendResetPacket();
    sendResetPacket();
    sendResetPacket();
    sendResetPacket();
    initCurrentValue = adcCurrentReadEvent();
    // initCurrentValue = readADCcachedValue(0);
    readCurrentFlag = true;
    progStat = false;
    currentStat = false;

    // printf("init current value: %d\n", initCurrentValue);

    sendDirectReadByte(cvNum, 255);
    if (currentStat)
    {
        progCVvalue = 255;
        progStat = true;
    }
    else
    {
        for (i = 0; i < 255; i++)
        {
            if (progProgressCb != nullptr)
            {
                progProgressCb(i, 255);
            }
            sendResetPacket();
            sendResetPacket();
            sendDirectReadByte(cvNum, i);
            if (currentStat)
            {
                progCVvalue = i;
                progStat = true;
                break;
            }
        }
    }

    readCurrentFlag = false;

    dccport::setPowerStat(false);
    dccport::setProgmode(false);
}

void progtrak::writeDirectModeByte(uint16_t cvNum, uint8_t writeValue)
{
    uint8_t i;

    dccport::setPowerStat(false);
    sleep_ms(100);
    dccport::setProgmode(true);
    dccport::setPowerStat(true);
    for (i = 0; i < 6; i++)
    {
        sendResetPacket();
    }
    initCurrentValue = readADCcachedValue(0);
    readCurrentFlag = true;
    currentStat = false;

    sendDirectWriteByte(cvNum, writeValue);

    for (i = 0; i < 6; i++)
    {
        sendResetPacket();
    }

    if (currentStat)
    {
        progCVvalue = writeValue;
        progStat = true;
    }
    else
    {
        progStat = false;
    }

    readCurrentFlag = false;
    dccport::setPowerStat(false);
    dccport::setProgmode(false);
}

void progtrak::writeDirectModeBit(uint16_t cvNum, uint8_t pos, bool value)
{
    uint8_t i;

    dccport::setPowerStat(false);
    sleep_ms(100);
    dccport::setProgmode(true);
    dccport::setPowerStat(true);
    for (i = 0; i < 6; i++)
    {
        sendResetPacket();
    }
    initCurrentValue = readADCcachedValue(0);
    readCurrentFlag = true;
    currentStat = false;

    sendDirectWriteBit(cvNum, pos, value);

    for (i = 0; i < 6; i++)
    {
        sendResetPacket();
    }

    if (currentStat)
    {
        progCVvalue = 1;
        progStat = true;
    }
    else
    {
        progStat = false;
    }

    readCurrentFlag = false;
    dccport::setPowerStat(false);
    dccport::setProgmode(false);
}

uint8_t progtrak::progMode;
uint16_t progtrak::progCVnum;
uint8_t progtrak::progCVvalue;

uint8_t progtrak::progCVbitpos;
bool progtrak::progCVbitValue;

uint16_t progtrak::initCurrentValue;
bool progtrak::progStat;

bool progtrak::currentStat;

void (*progtrak::progCb)(uint8_t, bool) = nullptr;
void (*progtrak::progProgressCb)(uint8_t, uint8_t) = nullptr;

void progtrak::setProgMode(uint8_t progModeValue)
{
    progMode &= 0x80;
    progMode |= progModeValue & 0x7F;
}

void progtrak::setReadWrite(uint8_t rwMode)
{
    progMode &= 0x7F;
    progMode |= rwMode & 0x80;
}

void progtrak::setCVnum(uint16_t cvNum)
{
    progCVnum = cvNum - 1;
}

void progtrak::setCVvalue(uint8_t cvValue)
{
    progCVvalue = cvValue;
}

void progtrak::setCVbitPos(uint8_t pos)
{
    progCVbitpos = pos;
}

void progtrak::setCVbitValue(bool data)
{
    progCVbitValue = data;
}

// Set Service-Mode Callback: Func(read-CV-value, status)
void progtrak::setCallback(void (*cb)(uint8_t, bool))
{
    progCb = cb;
}

void progtrak::setProgressCallback(void (*cb)(uint8_t, uint8_t))
{
    progProgressCb = cb;
}

void progtrak::RunProg(void)
{
    progModeFlag = true;
}

// Service-Mode Execute Event
void progtrak::EventProg(void)
{
    if (!progModeFlag)
        return;

    printf("prog mode\n");

    switch (progMode & 0x7F)
    {
    case PROGMODE_PAGED_BYTE:
        progModeFlag = false;
        break;
    case PROGMODE_DIRECT_BIT:
        if (progMode & PROGMODE_WRITE)
        {
            writeDirectModeBit(progCVnum, progCVbitpos, progCVbitValue);
            progModeFlag = false;
            if (progCb != nullptr)
            {
                progCb(0, progStat);
            }
        }
        else
        {
            readDirectModeBit(progCVnum);
            progModeFlag = false;
            if (progCb != nullptr)
            {
                progCb(progCVvalue, progStat);
            }
        }
        break;
    case PROGMODE_DIRECT_BYTE:
        if (progMode & PROGMODE_WRITE)
        {
            writeDirectModeByte(progCVnum, progCVvalue);
            progModeFlag = false;
            if (progCb != nullptr)
            {
                progCb(0, progStat);
            }
        }
        else
        {
            readDirectModeByte(progCVnum);
            progModeFlag = false;
            if (progCb != nullptr)
            {
                progCb(progCVvalue, progStat);
            }
        }
        break;
    default:
        progModeFlag = false;
        break;
    }

    // progModeFlag = false;
}