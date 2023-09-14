
#include "pico/stdlib.h"
#include "packetRouter.h"
#include "train_slot.h"
#include "../acc_ctrl.h"
#include <stdio.h>

void loconetPacketRouter::execLocoAdr()
{
    loconetTrainSlot *slotObj;
    uint16_t addr;
    uint8_t slotID;

    addr = (packetBuf[1] << 7) + (packetBuf[2]);

    if (addr > 0)
    {
        slotID = loconetTrainSlot::getSlotID(addr);
        //printf("getSlotID: trainaddr=%d, slotID=%d\n", addr, slotID);
        if (slotID == 0xFF)
        {
            // New Slot
            slotID = loconetTrainSlot::getFreeSlotID();
            if (slotID != 0xFF)
            {
                slotObj = loconetTrainSlot::getSlotObj(slotID);
                slotObj->writeTrainAddr(addr);
                // slotObj.writeTrainAddr(8);
                sendSlotReadData(slotID);
                return;
            }
            sendLongAck(LT_PACKET_LOCO_ADR, 1);
            return;
        }
        else
        {
            // slotObj = loconetTrainSlot::getSlotObj(slotID);
            // slotObj->writeTrainAddr(addr);
            slotObj->refreshTrainAddr();
            sendSlotReadData(slotID);
            return;
        }
    }
    sendLongAck(LT_PACKET_LOCO_ADR, 0);
}

void loconetPacketRouter::execMoveSlot()
{
    loconetTrainSlot *slotObj;

    if (loconetTrainSlot::validSlotID(packetBuf[1]))
    {
        if (packetBuf[1] == packetBuf[2])
        {
            // Change to In-Use
            slotObj = loconetTrainSlot::getSlotObj(packetBuf[1]);
            slotObj->setInUse();
            sendSlotReadData(packetBuf[1]);
            return;
        }
        else if (packetBuf[2] == 0x00)
        {
            // Change to Dispatch
            slotObj = loconetTrainSlot::getSlotObj(packetBuf[1]);
            slotObj->setIdle();
            sendSlotReadData(packetBuf[1]);
            return;
        }
    }

    // Failure
    sendLongAck(LT_PACKET_OPC_MOVE_SLOT, 0);
}

void loconetPacketRouter::execSlotRequestData()
{
    loconetTrainSlot *slotObj;

    if (loconetTrainSlot::validSlotID(packetBuf[1]))
    {
        sendSlotReadData(packetBuf[1]);
    }
    else
    {
    }
}

void loconetPacketRouter::execSlotWriteData(void)
{
    loconetTrainSlot *slotObj;

    if (loconetTrainSlot::validSlotID(packetBuf[2]))
    {
        slotObj = loconetTrainSlot::getSlotObj(packetBuf[2]);
        slotObj->setStat1(packetBuf[3]);
        slotObj->writeSpd(packetBuf[5]);
        slotObj->writeFunc1(packetBuf[6]);
        slotObj->setStat2(packetBuf[8]);
        slotObj->writeFunc2(packetBuf[10]);

        sendLongAck(LT_PACKET_SLOT_WRITE_DATA, 0);
    }
}

void loconetPacketRouter::execWriteSlot1()
{
    loconetTrainSlot *slotObj;

    if (loconetTrainSlot::validSlotID(packetBuf[1]))
    {
        slotObj = loconetTrainSlot::getSlotObj(packetBuf[1]);
        slotObj->setStat1(packetBuf[2]);
    }
}

void loconetPacketRouter::execLocoSpd()
{
    loconetTrainSlot *slotObj;
    if (loconetTrainSlot::validSlotID(packetBuf[1]))
    {
        slotObj = loconetTrainSlot::getSlotObj(packetBuf[1]);
        slotObj->writeSpd(packetBuf[2]);
    }
}

void loconetPacketRouter::execLocoDirf()
{
    loconetTrainSlot *slotObj;
    if (loconetTrainSlot::validSlotID(packetBuf[1]))
    {
        slotObj = loconetTrainSlot::getSlotObj(packetBuf[1]);
        slotObj->writeFunc1(packetBuf[2]);
    }
}

void loconetPacketRouter::execLocoSnd()
{
    loconetTrainSlot *slotObj;
    if (loconetTrainSlot::validSlotID(packetBuf[1]))
    {
        slotObj = loconetTrainSlot::getSlotObj(packetBuf[1]);
        slotObj->writeFunc2(packetBuf[2]);
    }
}

void loconetPacketRouter::execSwitchState()
{
    uint16_t addr;
    addr = packetBuf[1] + ((packetBuf[2] & 0x0F) << 7);

    if (accessoryCtrl::getAccessoryStat(addr))
    {
        // close response
        sendLongAck(LT_PACKET_SWITCH_STATE, 0x30);
    }
    else
    {
        // thrown response
        sendLongAck(LT_PACKET_SWITCH_STATE, 0x50);
    }
}

void loconetPacketRouter::execSwitchRequest()
{
    uint16_t addr;
    addr = packetBuf[1] + ((packetBuf[2] & 0x0F) << 7);

    if (!accessoryCtrl::sendAccessoryPacket(addr, packetBuf[2] & 0x20))
    {
        // Failure
        sendLongAck(LT_PACKET_SWITCH_REQUEST, 0);
    }
}