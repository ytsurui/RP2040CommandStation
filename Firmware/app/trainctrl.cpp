#include "pico/stdlib.h"
#include "trainctrl.h"
#include "trainpacket.h"
#include "dccsignal.h"

#include <stdio.h>

namespace {
uint16_t advanceSendCount(uint16_t count, uint16_t append)
{
    const uint32_t next = uint32_t(count) + append + 1;
    return next > PACKET_SEND_INTERVAL ? PACKET_SEND_INTERVAL + 1 : next;
}

// task() increments once before testing > PACKET_SEND_INTERVAL.
uint16_t remainingSendCount(uint16_t sendcount)
{
    return sendcount >= PACKET_SEND_INTERVAL
        ? 0
        : PACKET_SEND_INTERVAL - sendcount;
}
}

trainInfo trainctrl::trainCtrlData[TRAIN_CTRL_MAX];
bool trainctrl::enableTask;
uint16_t trainctrl::eventCounter;
uint16_t trainctrl::priorityEventCounter;
uint16_t trainctrl::smallLastCtrlCountValue; // 再度送信するまでの間隔のうち、最も小さいものを入れておく (パケットバッファが空になった際にすべての待機状態データにこの数値を足す)
uint16_t trainctrl::appendLastCtrlCountValue;

void trainctrl::init(void)
{
    uint16_t i;
    enableTask = false;
    eventCounter = priorityEventCounter = 0;
    smallLastCtrlCountValue = 0xFFFF;
    appendLastCtrlCountValue = 0;

    for (i = 0; i < TRAIN_CTRL_MAX; i++)
    {
        trainCtrlData[i].init();
    }
}

void trainctrl::eventMS(void)
{
    if (!enableTask)
    {
        enableTask = true;
        eventCounter = 0;
        priorityEventCounter = 0;

        // gpio_put(0, true);
        // gpio_put(0, false);

        if (dccsignal::getWaitPacketCount() == 0 && smallLastCtrlCountValue != 0xFFFF)
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

    // Failed operation enqueues get the next free capacity before periodic traffic.
    while (priorityEventCounter < TRAIN_CTRL_MAX) {
        if (!trainCtrlData[priorityEventCounter].retryPendingOperations()) {
            return;
        }
        ++priorityEventCounter;
    }

    if (eventCounter >= TRAIN_CTRL_MAX)
    {
        enableTask = false;
        return;
    }

    while(1) {
        lastCtrlValue = trainCtrlData[eventCounter].task(appendLastCtrlCountValue);
        eventCounter++;
        if (lastCtrlValue == 0xFFFF) {
            if (eventCounter >= TRAIN_CTRL_MAX)
            {
                enableTask = false;
                return;
            }
            continue;
        }

        if (smallLastCtrlCountValue > lastCtrlValue)
        {
            smallLastCtrlCountValue = lastCtrlValue;
        }
        break;
    }

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
    operationPending = false;
    lastCtrlCounter = 0;
    directionFlag = TRAIN_SPD_DIR_FOR;  // FOR

    origDir = TRAIN_SPD_DIR_FOR;
    origSpd = 0;

    // スロット再利用時に前の車両のデータが残らないよう、送信データも完全にクリアする
    clearTrainData(&trainData.speed14);
    clearTrainData(&trainData.speed28);
    clearTrainData(&trainData.speed128);
    clearTrainData(&trainData.FuncGroup1);
    clearTrainData(&trainData.FuncGroup2);
    clearTrainData(&trainData.FuncGroup3);
    clearTrainData(&trainData.FuncGroup4);
    clearTrainData(&trainData.FuncGroup5);
    clearTrainData(&trainData.FuncGroup6);
    clearTrainData(&trainData.FuncGroup7);
    clearTrainData(&trainData.FuncGroup8);
    clearTrainData(&trainData.FuncGroup9);
    clearTrainData(&trainData.FuncGroup10);

    robotDir = 0;   // NONE (1: toggle)
    robotSpd = 100;
}

void trainInfo::clearTrainData(trainDataInfo *td)
{
    td->enable = false;
    td->pending = false;
    td->data1 = 0;
    td->data2 = 0;
    td->sendcount = 0;
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
        trainData.speed14.sendcount = advanceSendCount(trainData.speed14.sendcount, appendWaitCount);

        if (trainData.speed14.sendcount > PACKET_SEND_INTERVAL)
        {
            if (trainpacket::sendSpeed14Packet(addr, trainData.speed14.data1, trainData.speed14.data2))
            {
                trainData.speed14.sendcount = 0;
            }
        }
        smallSendCount = remainingSendCount(trainData.speed14.sendcount);
    }
    else if (trainData.speed28.enable)
    {
        trainData.speed28.sendcount = advanceSendCount(trainData.speed28.sendcount, appendWaitCount);
        if (trainData.speed28.sendcount > PACKET_SEND_INTERVAL)
        {
            if (trainpacket::sendSpeed28Packet(addr, trainData.speed28.data1, trainData.speed28.data2))
            {
                trainData.speed28.sendcount = 0;
            }
        }
        smallSendCount = remainingSendCount(trainData.speed28.sendcount);
    }
    else if (trainData.speed128.enable)
    {
        // gpio_put(0, true);
        trainData.speed128.sendcount = advanceSendCount(trainData.speed128.sendcount, appendWaitCount);
        if (trainData.speed128.sendcount > PACKET_SEND_INTERVAL)
        {
            if (trainpacket::sendSpeed128Packet(addr, trainData.speed128.data1, trainData.speed128.data2))
            {
                trainData.speed128.sendcount = 0;
            }
        }
        smallSendCount = remainingSendCount(trainData.speed128.sendcount);
    }

    // gpio_put(0, true);

    if (trainData.FuncGroup1.enable)
    {
        trainData.FuncGroup1.sendcount = advanceSendCount(trainData.FuncGroup1.sendcount, appendWaitCount);
        if (trainData.FuncGroup1.sendcount > PACKET_SEND_INTERVAL)
        {
            if (trainpacket::sendFuncGroupPacket(addr, TRAIN_FUNC_BASIC_F0_F4, trainData.FuncGroup1.data1))
            {
                trainData.FuncGroup1.sendcount = 0;
            }
        }

        const uint16_t remaining = remainingSendCount(trainData.FuncGroup1.sendcount);
        if (smallSendCount > remaining)
        {
            smallSendCount = remaining;
        }
    }

    if (trainData.FuncGroup2.enable)
    {
        trainData.FuncGroup2.sendcount = advanceSendCount(trainData.FuncGroup2.sendcount, appendWaitCount);
        if (trainData.FuncGroup2.sendcount > PACKET_SEND_INTERVAL)
        {
            if (trainpacket::sendFuncGroupPacket(addr, TRAIN_FUNC_BASIC_F5_F8, trainData.FuncGroup2.data1))
            {
                trainData.FuncGroup2.sendcount = 0;
            }
        }

        const uint16_t remaining = remainingSendCount(trainData.FuncGroup2.sendcount);
        if (smallSendCount > remaining)
        {
            smallSendCount = remaining;
        }
    }

    if (trainData.FuncGroup3.enable)
    {
        trainData.FuncGroup3.sendcount = advanceSendCount(trainData.FuncGroup3.sendcount, appendWaitCount);
        if (trainData.FuncGroup3.sendcount > PACKET_SEND_INTERVAL)
        {
            if (trainpacket::sendFuncGroupPacket(addr, TRAIN_FUNC_BASIC_F9_F12, trainData.FuncGroup3.data1))
            {
                trainData.FuncGroup3.sendcount = 0;
            }
        }

        const uint16_t remaining = remainingSendCount(trainData.FuncGroup3.sendcount);
        if (smallSendCount > remaining)
        {
            smallSendCount = remaining;
        }
    }

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
        fg->sendcount = advanceSendCount(fg->sendcount, appendWaitCount);
        if (fg->sendcount > PACKET_SEND_INTERVAL) {
            if (trainpacket::sendExternalFuncPacket(addr, funcGroup, fg->data1)) {
                fg->sendcount = 0;
            }
        }

        const uint16_t remaining = remainingSendCount(fg->sendcount);
        if (*smallSendCount > remaining) {
            *smallSendCount = remaining;
        }
    }
}

bool trainInfo::sendItem(trainDataInfo *item, uint8_t kind)
{
    switch (kind) {
    case 0: return trainpacket::sendSpeed14Packet(addr, item->data1, item->data2);
    case 1: return trainpacket::sendSpeed28Packet(addr, item->data1, item->data2);
    case 2: return trainpacket::sendSpeed128Packet(addr, item->data1, item->data2);
    default:
        if (kind <= 5) return trainpacket::sendFuncGroupPacket(addr, kind - 2, item->data1);
        return trainpacket::sendExternalFuncPacket(addr, kind - 5, item->data1);
    }
}

void trainInfo::sendOperation(trainDataInfo *item, uint8_t kind)
{
    if (addr == 0) return;
    item->pending = !sendItem(item, kind);
    item->sendcount = item->pending ? PACKET_SEND_INTERVAL : 0;
    if (item->pending) {
        operationPending = true;
        // A command may arrive after this sweep's priority pass has finished.
        trainctrl::priorityEventCounter = 0;
    }
}

bool trainInfo::retryPendingOperations(void)
{
    if (!operationPending || addr == 0) return true;
    trainDataInfo *items[] = {
        &trainData.speed14, &trainData.speed28, &trainData.speed128,
        &trainData.FuncGroup1, &trainData.FuncGroup2, &trainData.FuncGroup3,
        &trainData.FuncGroup4, &trainData.FuncGroup5, &trainData.FuncGroup6,
        &trainData.FuncGroup7, &trainData.FuncGroup8, &trainData.FuncGroup9,
        &trainData.FuncGroup10
    };
    for (uint8_t kind = 0; kind < 13; ++kind) {
        trainDataInfo *item = items[kind];
        if (!item->enable) item->pending = false;
        if (!item->pending) continue;
        sendOperation(item, kind);
        if (item->pending) return false;
    }
    operationPending = false;
    return true;
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

    // 空きスロットの残留データを確実にクリアしてから割り当てる
    init();

    addr = newAddr;
    lastCtrlCounter = 0;

    return true;
}

void trainInfo::clearAddr(void)
{
    init();

    robotDir = 0;   // NONE (1: toggle)
    robotSpd = 100;
}

bool trainInfo::setSpeed14(uint8_t dir, uint8_t spd)
{
    if (dir != TRAIN_SPD_DIR_FOR && dir != TRAIN_SPD_DIR_REV) return false;
    origDir = dir;
    origSpd = spd;
    directionFlag = dir;    // ロボット反転前の指示方向を保持

    if (robotDir == 1) {
        // Robot Direction Control
        if (dir == 1) {
            dir = 2;
        } else {
            dir = 1;
        }
    }

    
    if (robotSpd < 100) {
        if (robotSpd == 0) {
            spd = 0;
        } else {
            spd = spd * robotSpd / 100;
        }
    }
    


    trainData.speed14.enable = true;
    trainData.speed128.enable = false;
    trainData.speed28.enable = false;

    trainData.speed14.data1 = dir;
    trainData.speed14.data2 = spd;
    lastCtrlCounter = 0;
    sendOperation(&trainData.speed14, 0);

    return true;
}

bool trainInfo::setSpeed28(uint8_t dir, uint8_t spd)
{
    if (dir != TRAIN_SPD_DIR_FOR && dir != TRAIN_SPD_DIR_REV) return false;
    origDir = dir;
    origSpd = spd;
    directionFlag = dir;    // ロボット反転前の指示方向を保持

    if (robotDir == 1) {
        // Robot Direction Control
        if (dir == 1) {
            dir = 2;
        } else {
            dir = 1;
        }
    }

    
    if (robotSpd < 100) {
        if (robotSpd == 0) {
            spd = 0;
        } else {
            spd = spd * robotSpd / 100;
        }
    }
    

    trainData.speed28.enable = true;
    trainData.speed14.enable = false;
    trainData.speed128.enable = false;

    trainData.speed28.data1 = dir;
    trainData.speed28.data2 = spd;
    lastCtrlCounter = 0;
    sendOperation(&trainData.speed28, 1);

    return true;
}

bool trainInfo::setSpeed128(uint8_t dir, uint8_t spd)
{
    if (dir != TRAIN_SPD_DIR_FOR && dir != TRAIN_SPD_DIR_REV) return false;
    if (spd > 127) return false;
    origDir = dir;
    origSpd = spd;
    directionFlag = dir;    // ロボット反転前の指示方向を保持

    if (robotDir == 1) {
        // Robot Direction Control
        if (dir == 1) {
            dir = 2;
        } else {
            dir = 1;
        }
    }

    
    if (robotSpd < 100) {
        if (robotSpd == 0) {
            spd = 0;
        } else {
            spd = spd * robotSpd / 100;
        }
    }
    

    // printf("trainInfo::setSpeed128 dir=%d, spd=%d\n", dir, spd);
    trainData.speed128.enable = true;
    trainData.speed14.enable = false;
    trainData.speed28.enable = false;

    trainData.speed128.data1 = dir;
    trainData.speed128.data2 = spd;
    lastCtrlCounter = 0;
    sendOperation(&trainData.speed128, 2);

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
    sendOperation(&trainData.FuncGroup1, 3);

    return true;
}

bool trainInfo::setFuncG2(uint8_t data)
{
    trainData.FuncGroup2.enable = true;
    trainData.FuncGroup2.data1 = data;
    lastCtrlCounter = 0;
    sendOperation(&trainData.FuncGroup2, 4);

    return true;
}

bool trainInfo::setFuncG3(uint8_t data)
{
    trainData.FuncGroup3.enable = true;
    trainData.FuncGroup3.data1 = data;
    lastCtrlCounter = 0;
    sendOperation(&trainData.FuncGroup3, 5);

    return true;
}

bool trainInfo::setFuncG4(uint8_t data)
{
    trainData.FuncGroup4.enable = true;
    trainData.FuncGroup4.data1 = data;
    lastCtrlCounter = 0;
    sendOperation(&trainData.FuncGroup4, 6);

    return true;
}

bool trainInfo::setFuncG5(uint8_t data)
{
    trainData.FuncGroup5.enable = true;
    trainData.FuncGroup5.data1 = data;
    lastCtrlCounter = 0;
    sendOperation(&trainData.FuncGroup5, 7);

    return true;
}

bool trainInfo::setFuncG6(uint8_t data)
{
    trainData.FuncGroup6.enable = true;
    trainData.FuncGroup6.data1 = data;
    lastCtrlCounter = 0;
    sendOperation(&trainData.FuncGroup6, 8);

    return true;
}

bool trainInfo::setFuncG7(uint8_t data)
{
    trainData.FuncGroup7.enable = true;
    trainData.FuncGroup7.data1 = data;
    lastCtrlCounter = 0;
    sendOperation(&trainData.FuncGroup7, 9);

    return true;
}

bool trainInfo::setFuncG8(uint8_t data)
{
    trainData.FuncGroup8.enable = true;
    trainData.FuncGroup8.data1 = data;
    lastCtrlCounter = 0;
    sendOperation(&trainData.FuncGroup8, 10);

    return true;
}

bool trainInfo::setFuncG9(uint8_t data)
{
    trainData.FuncGroup9.enable = true;
    trainData.FuncGroup9.data1 = data;
    lastCtrlCounter = 0;
    sendOperation(&trainData.FuncGroup9, 11);

    return true;
}

bool trainInfo::setFuncG10(uint8_t data)
{
    trainData.FuncGroup10.enable = true;
    trainData.FuncGroup10.data1 = data;
    lastCtrlCounter = 0;
    sendOperation(&trainData.FuncGroup10, 12);

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

bool trainInfo::setRobotDirection(uint8_t dir)
{
    if (dir >= 2) return false;

    if (robotDir == dir) {
        lastCtrlCounter = 0;
        return true;
    }

    robotDir = dir;
    lastCtrlCounter = 0;
    refreshTrainSpdDir();
    return true;
}

bool trainInfo::setRobotMaxSpd(uint8_t spd)
{
    if (spd > 100) return false;

    if (robotSpd == spd) {
        lastCtrlCounter = 0;
        return true;
    }
    robotSpd = spd;
    lastCtrlCounter = 0;
    refreshTrainSpdDir();
    return true;
}

uint8_t trainInfo::getRobotDirection()
{
    return robotDir;
}

uint8_t trainInfo::getRobotMaxSpd()
{
    return robotSpd;
}

void trainInfo::refreshTrainSpdDir()
{
    if (trainData.speed14.enable)
    {
        setSpeed14(origDir, origSpd);
    }
    else if (trainData.speed28.enable)
    {
        setSpeed28(origDir, origSpd);
    }
    else if (trainData.speed128.enable)
    {
        setSpeed128(origDir, origSpd);
    }
}