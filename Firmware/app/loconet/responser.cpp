
#include "pico/stdlib.h"
#include "packetRouter.h"
#include "train_slot.h"
#include <stdio.h>

loconetPacketRouter::cb_info loconetPacketRouter::cbdata;
loconetPacketRouter::cb_multidata loconetPacketRouter::cbSender2;

void loconetPacketRouter::setSender(void (*method)(uint8_t))
{
    cbdata.func = method;
    cbdata.assigned = true;
}

void loconetPacketRouter::setMultidataSender(void (*method)(uint8_t*, uint8_t))
{
    cbSender2.func = method;
    cbSender2.assigned = true;
}

void loconetPacketRouter::packetSend(uint8_t packet[], uint8_t length)
{
    uint8_t i;

    if (cbSender2.assigned) {
        cbSender2.func(packet, length);
        return;
    }

    if (cbdata.assigned)
    {
        printf("send: ");
        for (i = 0; i < length; i++)
        {
            printf("%02X ", packet[i]);
            cbdata.func(packet[i]);
        }
        printf("\r\n");
    }
}

void loconetPacketRouter::sendLongAck(uint8_t ackOpCode, uint8_t ackMsg)
{
    uint8_t data[4];

    data[0] = LT_PACKET_LONG_ACK;
    data[1] = ackOpCode & 0x7F;
    data[2] = ackMsg;
    data[3] = 0xFF ^ data[0] ^ data[1] ^ data[2];
    packetSend(data, 4);
}

void loconetPacketRouter::sendSlotReadData(uint8_t slotID)
{
    uint8_t data[14];
    uint16_t addr;
    uint8_t i;

    loconetTrainSlot *slotObj;

    slotObj = loconetTrainSlot::getSlotObj(slotID);
    addr = slotObj->getTrainAddr();
    // addr = 8;

    data[0] = LT_PACKET_SLOT_READ_DATA;
    data[1] = 0x0E;
    data[2] = slotID;
    data[3] = (slotObj->readStat1() & 0x7F);
    data[4] = addr & 0x007F;
    data[5] = slotObj->readSpd();
    data[6] = slotObj->readFunc1();
    data[7] = 0x07;
    data[8] = slotObj->readStat2();
    data[9] = (uint8_t)((addr >> 7) & 0x007F);
    data[10] = slotObj->readFunc2();
    data[11] = 0;
    data[12] = 0x55;

    data[13] = 0xFF;
    for (i = 0; i < 13; i++)
    {
        data[13] ^= data[i];
    }

    packetSend(data, 14);
}