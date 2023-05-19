
#include "pico/stdlib.h"
#include "train_slot.h"

// #include "../trainctrl.h"

void loconetTrainSlot::init(void)
{
    // trainctrl::trainctrlresp tInfo;

    if (trainAddr != 0)
    {
        // trainctrl::releaseTrain(trainAddr);
        trainctrl::releaseTrain(trainAddr);
    }

    slotStat1 = 0x80;
    slotStat2 = 0x00;
    direction = LOCO_DIR_FOR;
    timeoutCounter = 0;
    eventCounter = 0;
    trainAddr = 0;

    lastSendSpd = 0;
    lastSendDirection = 0;
    lastSendFunc1 = 0;
    lastSendFunc2 = 0;
    spdLastSendCounter = 0;
    func1LastSendCounter = 0;
    func2LastSendCounter = 0;
}

void loconetTrainSlot::event(void)
{
    /*
    trainctrl::trainctrlresp tInfo;

    eventCounter++;
    if (eventCounter == 10)
    {
        if ((lastSendSpd != spdValue) || (lastSendDirection != direction) || (spdLastSendCounter > LOCO_PACKET_RESEND_COUNT))
        {
            lastSendSpd = spdValue;
            lastSendDirection = direction;
            spdLastSendCounter = 0;
            tInfo = trainctrl::getTrainCtrl(trainAddr);
            if (tInfo.train != nullptr)
            {
                tInfo.train->setSpeed128(direction, spdValue);
            }
        }
        else
        {
            spdLastSendCounter++;
        }
    }
    else if (eventCounter == 20)
    {
        if ((lastSendFunc1 != func1Value) || (func1LastSendCounter > LOCO_PACKET_RESEND_COUNT))
        {
            lastSendFunc1 = func1Value;
            func1LastSendCounter = 0;
            tInfo = trainctrl::getTrainCtrl(trainAddr);
            if (tInfo.train != nullptr)
            {
                tInfo.train->setFuncG1(func1Value);
            }
        }
        else
        {
            func1LastSendCounter++;
        }
    }
    else if (eventCounter == 30)
    {
        if ((lastSendFunc2 != func2Value) || (func2LastSendCounter > LOCO_PACKET_RESEND_COUNT))
        {
            lastSendFunc2 = func2Value;
            func2LastSendCounter = 0;
            tInfo = trainctrl::getTrainCtrl(trainAddr);
            if (tInfo.train != nullptr)
            {
                tInfo.train->setFuncG2(func2Value);
            }
        }
        else
        {
            func2LastSendCounter++;
        }
    }
    else if (eventCounter > 30)
    {
        eventCounter = 0;
    }
    */
}

bool loconetTrainSlot::writeSpd(uint8_t spd)
{
    trainctrl::trainctrlresp tInfo;

    if (trainAddr == 0 || slotStat1 == 0x80)
    {
        return false;
    }

    tInfo = trainctrl::getTrainCtrl(trainAddr);
    if (!tInfo.enable)
    {
        // return false;
        refreshTrainAddr();
        tInfo = trainctrl::getTrainCtrl(trainAddr);
        if (!tInfo.enable)
        {
            return false;
        }
    }

    timeoutCounter = 0;

    if (tInfo.train != nullptr)
    {
        spdValue = spd;
        // return (tInfo.train->setSpeed128(direction, spd));
        return _writeSpd(&tInfo, spd);
    }
    return false;

    // return true;
}

bool loconetTrainSlot::_writeSpd(trainctrl::trainctrlresp *tInfo, uint8_t spd)
{
    uint8_t spdTmp;
    uint8_t dirTmp;

    switch (slotStat1 & 0x07)
    {
    case 0x00:
        // 28-step mode
        spdTmp = (spd * 32) / 128;
        if ((spdTmp > 0 && spdTmp < 4) && spd != 1)
        {
            spdTmp = 2;
        }
        return (tInfo->train->setSpeed28(direction, spdTmp));
    case 0x01:
        // Trinary Mode (Ignore 28-step)
        spdTmp = (spd * 32) / 128;
        if ((spdTmp > 0 && spdTmp < 4) && spd != 1)
        {
            spdTmp = 2;
        }
        return (tInfo->train->setSpeed28(direction, spdTmp));
    case 0x02:
        // 14-step mode
        spdTmp = (spd * 16) / 128;
        if (spdTmp == 1 && spd != 1)
        {
            spdTmp = 2;
        }
        return (tInfo->train->setSpeed14(direction, spdTmp));
    case 0x03:
        // 128-step mode
        // return (tInfo->train->setSpeed128(direction, spd));
        break;
    case 0x04:
        // 28-step mode (Allow Advanced DCC consisting)
        spdTmp = (spd * 32) / 128;
        if ((spdTmp > 0 && spdTmp < 4) && spd != 1)
        {
            spdTmp = 2;
        }
        return (tInfo->train->setSpeed28(direction, spdTmp));
    case 0x05:
        // Reserved 01 (Ignore 128-step)
        // return (tInfo->train->setSpeed128(direction, spd));
        break;
    case 0x06:
        // Reserved 02 (Ignore 128-step)
        // return (tInfo->train->setSpeed128(direction, spd));
        break;
    case 0x07:
        // 128-step mode (Allo Advanced DCC consisting)
        // return (tInfo->train->setSpeed128(direction, spd));
        break;
    }

    if (direction == LOCO_DIR_FOR)
    {
        dirTmp = LOCO_DIR_REV;
    }
    else
    {
        dirTmp = LOCO_DIR_FOR;
    }

    return (tInfo->train->setSpeed128(dirTmp, spd));
}

bool loconetTrainSlot::writeFunc1(uint8_t data)
{
    uint8_t oldDir;
    trainctrl::trainctrlresp tInfo;

    tInfo = trainctrl::getTrainCtrl(trainAddr);

    oldDir = direction;
    if (data & 0x20)
    {
        // Forward
        direction = LOCO_DIR_FOR;
    }
    else
    {
        direction = LOCO_DIR_REV;
    }

    if (oldDir != direction)
    {
        // tInfo.train->setSpeed128(direction, spdValue);
        _writeSpd(&tInfo, spdValue);
    }

    // func1Value = data & 0x1F;
    timeoutCounter = 0;
    if (tInfo.train != nullptr)
    {
        return (tInfo.train->setFuncG1(data & 0x1F));
    }
    return true;
}

/*
 *  writeFunc2: Write F5-F8 (snd)
 */
bool loconetTrainSlot::writeFunc2(uint8_t data)
{
    trainctrl::trainctrlresp tInfo;

    tInfo = trainctrl::getTrainCtrl(trainAddr);

    timeoutCounter = 0;
    if (tInfo.train != nullptr)
    {
        return tInfo.train->setFuncG2(data & 0x0F);
    }
    // func2Value = data & 0x0F;
    return true;
}

uint8_t loconetTrainSlot::readSpd(void)
{

    trainctrl::trainctrlresp tInfo;

    if (trainAddr == 0 || slotStat1 == 0x80)
    {
        return 0;
    }

    tInfo = trainctrl::getTrainCtrl(trainAddr);
    if (!tInfo.enable)
    {
        return 0;
    }

    // timeoutCounter = 0;
    // return (tInfo.train.getSpd);

    timeoutCounter = 0;
    return spdValue;
}

uint8_t loconetTrainSlot::readFunc1(void)
{
    trainctrl::trainctrlresp tInfo;

    if (trainAddr == 0 || slotStat1 == 0x80)
    {
        return 0;
    }

    tInfo = trainctrl::getTrainCtrl(trainAddr);
    if (!tInfo.enable)
    {
        return 0;
    }

    timeoutCounter = 0;
    return (tInfo.train->getFuncG1());

    // timeoutCounter = 0;
    // return func1Value;
}

uint8_t loconetTrainSlot::readFunc2(void)
{
    trainctrl::trainctrlresp tInfo;

    if (trainAddr == 0 || slotStat1 == 0x80)
    {
        return 0;
    }

    tInfo = trainctrl::getTrainCtrl(trainAddr);
    if (!tInfo.enable)
    {
        return 0;
    }

    timeoutCounter = 0;
    return (tInfo.train->getFuncG2());

    // timeoutCounter = 0;
    // return func2Value;
}

uint16_t loconetTrainSlot::getTrainAddr()
{
    return trainAddr;
}

void loconetTrainSlot::writeTrainAddr(uint16_t addr)
{
    trainctrl::trainctrlresp tInfo;

    trainAddr = addr;
    slotStat1 &= ~0x80;
    slotStat1 |= 0x13; // 128-speed step

    timeoutCounter = 0;
    tInfo = trainctrl::selectNewTrain(trainAddr);
}

void loconetTrainSlot::refreshTrainAddr(void)
{
    trainctrl::trainctrlresp tInfo;

    tInfo = trainctrl::getTrainCtrl(trainAddr);
    if (!tInfo.enable)
    {
        tInfo = trainctrl::selectNewTrain(trainAddr);
    }
}

void loconetTrainSlot::setInUse(void)
{
    if (trainAddr == 0 || slotStat1 == 0x80)
    {
        return;
    }

    // slotStat1 &= ~0x18;
    // slotStat1 |= 0x18;
    slotStat1 = 0x33;
}

void loconetTrainSlot::setIdle(void)
{
    if (trainAddr == 0 || slotStat1 == 0x80)
    {
        return;
    }
    slotStat1 &= ~0x08;
    slotStat1 |= 0x10;
}

void loconetTrainSlot::setCommon(void)
{
    if (trainAddr == 0 || slotStat1 == 0x80)
    {
        return;
    }
    slotStat1 &= ~0x10;
    slotStat1 |= 0x08;
}

uint8_t loconetTrainSlot::readStat1(void)
{
    return slotStat1;
}

uint8_t loconetTrainSlot::readStat2(void)
{
    return slotStat2;
}

void loconetTrainSlot::setStat1(uint8_t data)
{
    slotStat1 = data;
}

void loconetTrainSlot::setStat2(uint8_t data)
{
    slotStat2 = data;
}