#include "pico/stdlib.h"
#include "trainctrl.h"
#include "trainpacket.h"
#include "dccsignal.h"

#include "../throttle/peripheral/lcd.h"

#include <stdio.h>

trainInfo trainctrl::trainCtrlData[TRAIN_CTRL_MAX];
bool trainctrl::enableTask;
uint16_t trainctrl::eventCounter;
uint16_t trainctrl::smallLastCtrlCountValue; // 再度送信するまでの間隔のうち、最も小さいものを入れておく (パケットバッファが空になった際にすべての待機状態データにこの数値を足す)
uint16_t trainctrl::appendLastCtrlCountValue;

void trainctrl::init(void)
{
    uint16_t i;

    for (i = 0; i < TRAIN_CTRL_MAX; i++)
    {
        trainCtrlData->init();
    }
}

void trainctrl::eventMS(void)
{
    if (!enableTask)
    {
        enableTask = true;
        eventCounter = 0;

        // gpio_put(0, true);
        // gpio_put(0, false);

        if (dccsignal::getWaitPacketCount() == 0)
        {
            appendLastCtrlCountValue = smallLastCtrlCountValue;
            // appendLastCtrlCountValue = 0;
        }
        else
        {
            appendLastCtrlCountValue = 0;
        }

        smallLastCtrlCountValue = 0xFFFF;
    }
}

void trainctrl::task(void)
{
    uint16_t lastCtrlValue;

    if (!enableTask)
    {
        return;
    }

    if (eventCounter >= TRAIN_CTRL_MAX)
    {
        enableTask = false;
        return;
    }

    lastCtrlValue = trainCtrlData[eventCounter].task(appendLastCtrlCountValue);
    if (smallLastCtrlCountValue > lastCtrlValue)
    {
        smallLastCtrlCountValue = lastCtrlValue;
    }
    eventCounter++;

    if (eventCounter >= TRAIN_CTRL_MAX)
    {
        enableTask = false;
        return;
    }
}

void trainctrl::releaseTrain(uint16_t addr)
{
    uint16_t i, tempAddr;

    for (i = 0; i < TRAIN_CTRL_MAX; i++)
    {
        tempAddr = trainCtrlData[i].getAddr();
        if (tempAddr == addr)
        {
            printf("release train, addr=%d\n", addr);
            trainCtrlData[i].init();
            return;
        }
    }
}

trainctrl::trainctrlresp trainctrl::selectNewTrain(uint16_t addr)
{
    uint16_t i, tempAddr, newSlotIndex;
    trainctrlresp respData;
    respData.enable = false;
    respData.train = nullptr;

    if (addr == 0)
    {
        return (respData);
    }

    newSlotIndex = 0xFFFF;
    for (i = 0; i < TRAIN_CTRL_MAX; i++)
    {
        tempAddr = trainCtrlData[i].getAddr();
        if (tempAddr == addr)
        {
            respData.enable = true;
            respData.train = &trainCtrlData[i];
            //printf("selectNewTrain: Get Exist TrainCtrlData: addr=%d, index=%d\n", addr, i);
            return (respData);
        }
        else if (tempAddr == 0)
        {
            if (newSlotIndex > i)
            {
                newSlotIndex = i;
            }
        }
    }

    if (newSlotIndex == 0xFFFF)
    {
        return (respData);
    }
    //printf("selectNewTrain: Get New TrainCtrlData: addr=%d, index=%d\n", addr, newSlotIndex);
    trainCtrlData[newSlotIndex].setAddr(addr);
    respData.enable = true;
    respData.train = &trainCtrlData[newSlotIndex];
    return (respData);
}

trainctrl::trainctrlresp trainctrl::getTrainCtrl(uint16_t addr)
{
    uint16_t i;
    trainctrlresp respData;

    respData.enable = false;
    respData.train = nullptr;

    if (addr == 0)
    {
        return (respData);
    }

    for (i = 0; i < TRAIN_CTRL_MAX; i++)
    {
        if (trainCtrlData[i].getAddr() == addr)
        {
            respData.enable = true;
            respData.train = &trainCtrlData[i];
            return (respData);
        }
    }

    return respData;
}

trainctrl::trainctrlresp trainctrl::getTrainCtrlWithNewObj(uint16_t addr)
{
    trainctrlresp respData;

    respData = getTrainCtrl(addr);
    if (respData.enable) {
        return respData;
    }

    respData = selectNewTrain(addr);
    return respData;
}

void trainInfo::init(void)
{
    // printf("trainInfo init: lastAddr=%d\n", addr);
    addr = 0;
    lastCtrlCounter = 0;
    directionFlag = 1;  // FOR
    trainData.speed14.enable = false;
    trainData.speed28.enable = false;
    trainData.speed128.enable = false;
    trainData.FuncGroup1.enable = false;
    trainData.FuncGroup2.enable = false;
    trainData.FuncGroup3.enable = false;
    trainData.FuncGroup4.enable = false;
    trainData.FuncGroup5.enable = false;
    trainData.FuncGroup6.enable = false;
    trainData.FuncGroup7.enable = false;
    trainData.FuncGroup8.enable = false;
    trainData.FuncGroup9.enable = false;
    trainData.FuncGroup10.enable = false;
}

uint16_t trainInfo::task(uint16_t appendWaitCount)
{
    uint16_t smallSendCount;
    smallSendCount = 0xFFFF;

    if (addr == 0)
    {
        return (smallSendCount);
    }

    lastCtrlCounter++;
    if (lastCtrlCounter > TRAIN_TIMEOUT_MS)
    {
        // Release
        // printf("train timeout: addr=%d\n", addr);
        init();
        return (smallSendCount);
    }

    // gpio_put(0, false);

    if (trainData.speed14.enable)
    {
        if (appendWaitCount > 0)
        {
            trainData.speed14.sendcount += appendWaitCount;
        }
        trainData.speed14.sendcount++;

        if (trainData.speed14.sendcount > PACKET_SEND_INTERVAL)
        {
            if (trainpacket::sendSpeed14Packet(addr, trainData.speed14.data1, trainData.speed14.data2))
            {
                trainData.speed14.sendcount = 0;
            }
        }
        smallSendCount = trainData.speed14.sendcount;
    }
    else if (trainData.speed28.enable)
    {
        if (appendWaitCount > 0)
        {
            trainData.speed28.sendcount += appendWaitCount;
        }
        trainData.speed28.sendcount++;
        if (trainData.speed28.sendcount > PACKET_SEND_INTERVAL)
        {
            if (trainpacket::sendSpeed28Packet(addr, trainData.speed28.data1, trainData.speed28.data2))
            {
                trainData.speed28.sendcount = 0;
            }
        }
        smallSendCount = trainData.speed28.sendcount;
    }
    else if (trainData.speed128.enable)
    {
        // gpio_put(0, true);
        if (appendWaitCount > 0)
        {
            // gpio_put(0, true);
            trainData.speed128.sendcount += appendWaitCount;
        }
        trainData.speed128.sendcount++;
        if (trainData.speed128.sendcount > PACKET_SEND_INTERVAL)
        {
            if (trainpacket::sendSpeed128Packet(addr, trainData.speed128.data1, trainData.speed128.data2))
            {
                trainData.speed128.sendcount = 0;
            }
        }
        smallSendCount = trainData.speed128.sendcount;
    }

    // gpio_put(0, true);

    if (trainData.FuncGroup1.enable)
    {
        if (appendWaitCount > 0)
        {
            trainData.FuncGroup1.sendcount += appendWaitCount;
        }
        trainData.FuncGroup1.sendcount++;
        if (trainData.FuncGroup1.sendcount > PACKET_SEND_INTERVAL)
        {
            if (trainpacket::sendFuncGroupPacket(addr, TRAIN_FUNC_BASIC_F0_F4, trainData.FuncGroup1.data1))
            {
                trainData.FuncGroup1.sendcount = 0;
            }
        }

        if (smallSendCount > trainData.FuncGroup1.sendcount)
        {
            smallSendCount = trainData.FuncGroup1.sendcount;
        }
    }

    if (trainData.FuncGroup2.enable)
    {
        if (appendWaitCount > 0)
        {
            trainData.FuncGroup2.sendcount += appendWaitCount;
        }
        trainData.FuncGroup2.sendcount++;
        if (trainData.FuncGroup2.sendcount > PACKET_SEND_INTERVAL)
        {
            if (trainpacket::sendFuncGroupPacket(addr, TRAIN_FUNC_BASIC_F5_F8, trainData.FuncGroup2.data1))
            {
                trainData.FuncGroup2.sendcount = 0;
            }
        }

        if (smallSendCount > trainData.FuncGroup2.sendcount)
        {
            smallSendCount = trainData.FuncGroup2.sendcount;
        }
    }

    if (trainData.FuncGroup3.enable)
    {
        if (appendWaitCount > 0)
        {
            trainData.FuncGroup3.sendcount += appendWaitCount;
        }
        trainData.FuncGroup3.sendcount++;
        if (trainData.FuncGroup3.sendcount > PACKET_SEND_INTERVAL)
        {
            if (trainpacket::sendFuncGroupPacket(addr, TRAIN_FUNC_BASIC_F9_F12, trainData.FuncGroup3.data1))
            {
                trainData.FuncGroup3.sendcount = 0;
            }
        }

        if (smallSendCount > trainData.FuncGroup3.sendcount)
        {
            smallSendCount = trainData.FuncGroup3.sendcount;
        }
    }
    /*
    if (trainData.FuncGroup4.enable)
    {
        if (appendWaitCount > 0)
        {
            trainData.FuncGroup4.sendcount += appendWaitCount;
        }
        trainData.FuncGroup4.sendcount++;
        if (trainData.FuncGroup4.sendcount > PACKET_SEND_INTERVAL)
        {
            if (trainpacket::sendExternalFuncPacket(addr, TRAIN_FUNC_EXTERNAL_F13_F20, trainData.FuncGroup4.data1))
            {
                trainData.FuncGroup4.sendcount = 0;
            }
        }

        if (smallSendCount > trainData.FuncGroup4.sendcount)
        {
            smallSendCount = trainData.FuncGroup4.sendcount;
        }
    }

    if (trainData.FuncGroup5.enable)
    {
        if (appendWaitCount > 0)
        {
            trainData.FuncGroup5.sendcount += appendWaitCount;
        }
        trainData.FuncGroup5.sendcount++;
        if (trainData.FuncGroup5.sendcount > PACKET_SEND_INTERVAL)
        {
            if (trainpacket::sendExternalFuncPacket(addr, TRAIN_FUNC_EXTERNAL_F20_F28, trainData.FuncGroup5.data1))
            {
                trainData.FuncGroup5.sendcount = 0;
            }
        }

        if (smallSendCount > trainData.FuncGroup5.sendcount)
        {
            smallSendCount = trainData.FuncGroup5.sendcount;
        }
    }
    */

    funcSendStub(&trainData.FuncGroup4, appendWaitCount, &smallSendCount, TRAIN_FUNC_EXTERNAL_F13_F20);
    funcSendStub(&trainData.FuncGroup5, appendWaitCount, &smallSendCount, TRAIN_FUNC_EXTERNAL_F20_F28);
    funcSendStub(&trainData.FuncGroup6, appendWaitCount, &smallSendCount, TRAIN_FUNC_EXTERNAL_F29_F36);
    funcSendStub(&trainData.FuncGroup7, appendWaitCount, &smallSendCount, TRAIN_FUNC_EXTERNAL_F37_F44);
    funcSendStub(&trainData.FuncGroup8, appendWaitCount, &smallSendCount, TRAIN_FUNC_EXTERNAL_F45_F52);
    funcSendStub(&trainData.FuncGroup9, appendWaitCount, &smallSendCount, TRAIN_FUNC_EXTERNAL_F53_F60);
    funcSendStub(&trainData.FuncGroup10, appendWaitCount, &smallSendCount, TRAIN_FUNC_EXTERNAL_F61_F68);

    return (smallSendCount);
}

void trainInfo::funcSendStub(trainDataInfo *fg, uint16_t appendWaitCount, uint16_t *smallSendCount, uint8_t funcGroup)
{
    if (fg->enable) {
        if (appendWaitCount > 0) {
            fg->sendcount = appendWaitCount;
        }
        fg->sendcount++;
        if (fg->sendcount > PACKET_SEND_INTERVAL) {
            if (trainpacket::sendExternalFuncPacket(addr, funcGroup, fg->data1)) {
                fg->sendcount = 0;
            }
        }

        if (*smallSendCount > fg->sendcount) {
            *smallSendCount = fg->sendcount;
        }
    }
}

uint16_t trainInfo::getAddr(void)
{
    return (addr);
}

bool trainInfo::setAddr(uint16_t newAddr)
{
    if (addr != 0)
    {
        return false;
    }
    addr = newAddr;
    lastCtrlCounter = 0;
    return true;
}

void trainInfo::clearAddr(void)
{
    init();
}

bool trainInfo::setSpeed14(uint8_t dir, uint8_t spd)
{
    trainData.speed14.enable = true;
    trainData.speed128.enable = false;
    trainData.speed28.enable = false;

    trainData.speed14.data1 = dir;
    trainData.speed14.data2 = spd;
    lastCtrlCounter = 0;

    return true;
}

bool trainInfo::setSpeed28(uint8_t dir, uint8_t spd)
{
    trainData.speed28.enable = true;
    trainData.speed14.enable = false;
    trainData.speed128.enable = false;

    trainData.speed28.data1 = dir;
    trainData.speed28.data2 = spd;
    lastCtrlCounter = 0;

    return true;
}

bool trainInfo::setSpeed128(uint8_t dir, uint8_t spd)
{
    // printf("trainInfo::setSpeed128 dir=%d, spd=%d\n", dir, spd);
    trainData.speed128.enable = true;
    trainData.speed14.enable = false;
    trainData.speed28.enable = false;

    trainData.speed128.data1 = dir;
    trainData.speed128.data2 = spd;
    lastCtrlCounter = 0;

    // trainpacket::sendSpeed128Packet(addr, trainData.speed128.data1, trainData.speed128.data2);
    // gpio_put(0, true);

    return true;
}

bool trainInfo::getSpeedType(uint8_t *spd, uint8_t *spdType)
{
    if (trainData.speed128.enable) {
        *spd = trainData.speed128.data2;
        *spdType = 2;
    } else if (trainData.speed28.enable) {
        *spd = trainData.speed28.data2;
        *spdType = 1;
    } else if (trainData.speed14.enable) {
        *spd = trainData.speed14.data2;
        *spdType = 0;
    } else {
        *spd = 0;
        *spdType = 2;
        return false;
    }
    return true;
}

bool trainInfo::setFuncG1(uint8_t data)
{
    trainData.FuncGroup1.enable = true;
    trainData.FuncGroup1.data1 = data;
    lastCtrlCounter = 0;

    return true;
}

bool trainInfo::setFuncG2(uint8_t data)
{
    trainData.FuncGroup2.enable = true;
    trainData.FuncGroup2.data1 = data;
    lastCtrlCounter = 0;

    return true;
}

bool trainInfo::setFuncG3(uint8_t data)
{
    trainData.FuncGroup3.enable = true;
    trainData.FuncGroup3.data1 = data;
    lastCtrlCounter = 0;

    return true;
}

bool trainInfo::setFuncG4(uint8_t data)
{
    trainData.FuncGroup4.enable = true;
    trainData.FuncGroup4.data1 = data;
    lastCtrlCounter = 0;

    return true;
}

bool trainInfo::setFuncG5(uint8_t data)
{
    trainData.FuncGroup5.enable = true;
    trainData.FuncGroup5.data1 = data;
    lastCtrlCounter = 0;

    return true;
}

bool trainInfo::setFuncG6(uint8_t data)
{
    trainData.FuncGroup6.enable = true;
    trainData.FuncGroup6.data1 = data;
    lastCtrlCounter = 0;

    return true;
}

bool trainInfo::setFuncG7(uint8_t data)
{
    trainData.FuncGroup7.enable = true;
    trainData.FuncGroup7.data1 = data;
    lastCtrlCounter = 0;

    return true;
}

bool trainInfo::setFuncG8(uint8_t data)
{
    trainData.FuncGroup8.enable = true;
    trainData.FuncGroup8.data1 = data;
    lastCtrlCounter = 0;

    return true;
}

bool trainInfo::setFuncG9(uint8_t data)
{
    trainData.FuncGroup9.enable = true;
    trainData.FuncGroup9.data1 = data;
    lastCtrlCounter = 0;

    return true;
}

bool trainInfo::setFuncG10(uint8_t data)
{
    trainData.FuncGroup10.enable = true;
    trainData.FuncGroup10.data1 = data;
    lastCtrlCounter = 0;

    return true;
}

uint8_t trainInfo::getFuncG1(void)
{
    return (trainData.FuncGroup1.data1);
}

uint8_t trainInfo::getFuncG2(void)
{
    return (trainData.FuncGroup2.data1);
}

uint8_t trainInfo::getFuncG3(void)
{
    return (trainData.FuncGroup3.data1);
}

uint8_t trainInfo::getFuncG4(void)
{
    return (trainData.FuncGroup4.data1);
}

uint8_t trainInfo::getFuncG5(void)
{
    return (trainData.FuncGroup5.data1);
}

uint8_t trainInfo::getFuncG6(void)
{
    return (trainData.FuncGroup6.data1);
}

uint8_t trainInfo::getFuncG7(void)
{
    return (trainData.FuncGroup7.data1);
}

uint8_t trainInfo::getFuncG8(void)
{
    return (trainData.FuncGroup8.data1);
}

uint8_t trainInfo::getFuncG9(void)
{
    return (trainData.FuncGroup9.data1);
}

uint8_t trainInfo::getFuncG10(void)
{
    return (trainData.FuncGroup10.data1);
}

uint8_t trainInfo::getDirFlag(void)
{
    return directionFlag;
}

void trainInfo::setDirFlag(uint8_t dir)
{
    directionFlag = dir;
}