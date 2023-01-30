
#include "pico/stdlib.h"
#include "dccsignal.h"
#include "acc_ctrl.h"

#include <stdio.h>

#define SWITCH_PACKET_SEND_RETRY_COUNT 4

uint32_t accessoryCtrl::accStatCache[64];
uint16_t accessoryCtrl::addrQueue[ACC_ADDR_QUEUE_SIZE];
uint8_t accessoryCtrl::queueInPos;
uint8_t accessoryCtrl::queueOutPos;

void accessoryCtrl::init()
{
    uint8_t i;

    for (i = 0; i < ACC_ADDR_QUEUE_SIZE; i++)
    {
        addrQueue[i] = 0xFFFF;
    }
    queueInPos = 0;
    queueOutPos = 0;
}

void accessoryCtrl::task()
{
    uint8_t i;
    uint16_t accAddr;
    bool packetStat;

    if (queueInPos == queueOutPos)
        return;

    /*
    for (i = 0; i < ACC_ADDR_QUEUE_SIZE; i++)
    {
        if (addrQueue[i] == 0xFFFF)
            continue;
    }
    */

    while (1)
    {
        if (queueInPos == queueOutPos)
            break;

        if (addrQueue[queueOutPos] == 0xFFFF)
        {
            queueOutPos++;
            continue;
        }

        accAddr = addrQueue[queueOutPos] & 0x3FFF;

        packetStat = sendAccPacketData(accAddr, addrQueue[queueOutPos] & 0x4000);

        if (packetStat)
        {
            addrQueue[queueOutPos] = 0xFFFF; // Clear Queue
            queueOutPos++;

            if (queueOutPos >= ACC_ADDR_QUEUE_SIZE)
            {
                queueOutPos = 0;
            }
        }
        else
        {
            break;
        }
    }
}

bool accessoryCtrl::sendAccessoryPacket(uint16_t accAddr, bool stat)
{
    bool packetStat;

    packetStat = sendAccPacketData(accAddr, stat);

    if (!packetStat)
    {
        addrQueue[queueInPos] = accAddr;

        if (stat)
        {
            addrQueue[queueInPos] |= 0x4000;
        }
        queueInPos++;

        if (queueInPos >= ACC_ADDR_QUEUE_SIZE)
        {
            queueInPos = 0;
        }
    }

    return packetStat;
}

bool accessoryCtrl::sendAccPacketData(uint16_t accAddr, bool stat)
{
    uint8_t packet[3];
    uint16_t addrTmp;
    uint8_t int_t;
    bool packetStat;

    // printf("send switch: %d, stat:%d\n", accAddr, stat);

    // accAddr: 1-2044 (11bit)

    // packet[0] = 0x80 + ((accAddr >> 5) & 0x3F);
    // packet[1] = 0x88 | ((accAddr & 0x001C) << 2) | ((accAddr & 0x03) << 1);

    // accAddr: 0b 0000 0abc defg hijk
    // packet[0]: 10de fghi (10AAAAAA)
    // packet[1]: 1abc XjkY (1AAACDDD)
    //
    // X: 1
    // Y: stat

    // packet[0] = 0x80 | ((accAddr >> 2) & 0x3F);
    // packet[1] = 0x88 | ((((~accAddr >> 2) & 0x8C0) >> 2) & 0x70) | (accAddr & 0x03) << 1;
    addrTmp = (accAddr >> 2) + 1;

    // packet[0] = 0x80 | ((accAddr >> 2) & 0x3F);
    // packet[1] = 0xF8 | ((accAddr & 0x03) << 1);
    int_t = (((~addrTmp >> 6) & 0x07));

    // packet[0] = 0x80 | (addrTmp & 0x3F);
    // packet[0] = 0x80 | int_t;
    // packet[0] = 0x80;
    // packet[1] = 0xF8 | ((accAddr & 0x03) << 1);
    // packet[1] = 0x80 | ((int_t & 0x07) << 4);
    // packet[1] = 0xF8;

    packet[0] = 0x80 | (addrTmp & 0x3F);
    packet[1] = 0x88 | (int_t << 4) | ((accAddr & 0x03) << 1);

    // addrTmp = accAddr >> 2;
    // packet[0] = 0x80 | (addrTmp & 0x3F);
    // packet[1] = 0x88 | ((~addrTmp & 0x08C0) >> 2);

    if (stat)
    {
        // true = closed
        packet[1] |= 0x01;
    }

    dccsignal::calcChecksumPacket(packet, 2);
    // packetStat = dccsignal::putPacket(packet, 3, 2, accAddr, 0x40);
    packetStat = dccsignal::putPacket(packet, 3, SWITCH_PACKET_SEND_RETRY_COUNT, accAddr, 0x40);
    // printf("%d\n", packetStat);

    if (packetStat)
    {
        setAccessoryStat(accAddr, stat);
    }
    return packetStat;
}

bool accessoryCtrl::getAccessoryStat(uint16_t accAddr)
{
    accessoryCtrl::AccAddrCache addrPos;

    addrPos = encodeAccAddr(accAddr);
    if (addrPos.bitpos == 0)
    {
        return true;
    }

    if (accStatCache[addrPos.index] & addrPos.bitpos)
    {
        return false;
    }
    return true;
}

void accessoryCtrl::setAccessoryStat(uint16_t accAddr, bool stat)
{
    accessoryCtrl::AccAddrCache addrPos;

    addrPos = encodeAccAddr(accAddr);
    if (addrPos.bitpos == 0)
    {
        return;
    }

    if (!stat)
    {
        accStatCache[addrPos.index] |= addrPos.bitpos;
    }
    else
    {
        accStatCache[addrPos.index] &= ~addrPos.bitpos;
    }
}

accessoryCtrl::AccAddrCache accessoryCtrl::encodeAccAddr(uint16_t accAddr)
{
    accessoryCtrl::AccAddrCache respData;

    if (accAddr > 2044)
    {
        respData.bitpos = 0;
        respData.index = 0;
        return respData;
    }

    // accAddr -= 1;

    respData.index = (accAddr / 32);
    respData.bitpos = (0x01 << (accAddr % 32));

    return respData;
}