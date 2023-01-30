
#include "pico/stdlib.h"
#include "packetRouter.h"
#include "train_slot.h"

#include <stdio.h>

#include "../../peripheral/dccsignalport.h"

uint8_t loconetPacketRouter::packetBuf[LOCONET_PACKET_BUF_MAX];
uint8_t loconetPacketRouter::bufpos;
uint8_t loconetPacketRouter::packetLength;
uint8_t loconetPacketRouter::checksum;

void loconetPacketRouter::init(void)
{
    bufpos = 0;
    checksum = 0;
    packetLength = 0;

    loconetTrainSlot::globalInit();
}

void loconetPacketRouter::event(void)
{
    loconetTrainSlot::globalEventMS();
}

void loconetPacketRouter::recv(uint8_t packetByte)
{
    if (packetByte & 0x80)
    {
        // New Packet Start
        bufpos = 0;
        checksum = 0;
        switch (packetByte & 0x60)
        {
        case 0x00: // 1-Byte Packet
            packetLength = 1;
            break;
        case 0x20: // 3-Byte Packet
            packetLength = 3;
            break;
        case 0x40: // 5-Byte Packet
            packetLength = 5;
            break;
        case 0x60: // Variable Length Packet
            packetLength = 0xFF;
            break;
        default:
            packetLength = 0;
        }
    }

    if ((packetLength == 0xFF) && (bufpos == 1))
    {
        packetLength = packetByte - 1;
    }

    packetBuf[bufpos] = packetByte;
    checksum ^= packetByte;
    bufpos++;

    if (bufpos > packetLength)
    {
        if (checksum == 0xFF)
        {
            // packet echo
            // packetSend(packetBuf, packetLength + 1);

            // exec
            packetExec();
        }
    }
}

void loconetPacketRouter::packetExec()
{
    uint8_t i;

    if (packetLength == 0)
        return;

    printf("recv: ");
    for (i = 0; i <= packetLength; i++)
    {
        printf("%02X ", packetBuf[i]);
    }
    printf("\r\n");

    switch (packetBuf[0])
    {
    case LT_PACKET_GP_OFF:
        dccport::setPowerStat(false);
        break;
    case LT_PACKET_GP_ON:
        dccport::setPowerStat(true);
        break;
    case LT_PACKET_LOCO_ADR:
        execLocoAdr();
        break;
    case LT_PACKET_REQUEST_SLOT_DATA:
        execSlotRequestData();
        break;
    case LT_PACKET_LOCO_SPD:
        execLocoSpd();
        break;
    case LT_PACKET_LOCO_DIRF:
        execLocoDirf();
        break;
    case LT_PACKET_LOCO_SND:
        execLocoSnd();
        break;
    case LT_PACKET_SLOT_STAT1:
        execWriteSlot1();
        break;
    case LT_PACKET_SLOT_READ_DATA:
        break;
    case LT_PACKET_SLOT_WRITE_DATA: // 0xEF
        execSlotWriteData();
        break;
    case LT_PACKET_OPC_MOVE_SLOT: // 0xBA
        execMoveSlot();
        break;
    case LT_PACKET_SWITCH_STATE:
        execSwitchState();
        break;
    case LT_PACKET_SWITCH_REQUEST:
        execSwitchRequest();
        break;
    }
}