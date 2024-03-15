
#include "pico/stdlib.h"
#include "dccsignal.h"
#include "trainpacket.h"

#define TRAIN_PACKET_SEND_RETRY_COUNT 3

uint8_t trainpacket::addrMode = TRAIN_ADDR_MODE_US;

trainpacket::TrainAddrDecoded trainpacket::decodeTrainAddr(uint16_t trainAddr, uint8_t mode)
{
    trainpacket::TrainAddrDecoded respData;

    respData.HighAddrPacket = 0;
    respData.LowAddrPacket = 0;
    respData.failure = false;

    if (trainAddr == 0)
    {
        respData.failure = true;
        return respData;
    }

    if (mode == TRAIN_ADDR_MODE_EU)
    {
        if (trainAddr <= 100)
        {
            respData.HighAddrPacket = 0;
            respData.LowAddrPacket = trainAddr;
            return respData;
        }
    }
    else if (mode == TRAIN_ADDR_MODE_US)
    {
        if (trainAddr <= 127)
        {
            respData.HighAddrPacket = 0;
            respData.LowAddrPacket = trainAddr;
            return respData;
        }
    }
    else
    {
        return respData;
    }

    // respData.HighAddrPacket = 0x80 + ((trainAddr & 0x3F00) >> 8);
    respData.HighAddrPacket = 0xC0 + ((trainAddr >> 8) & 0x3F);
    respData.LowAddrPacket = trainAddr & 0x00FF;
    return respData;
}

bool trainpacket::sendSpeed28Packet(uint16_t trainAddr, uint8_t direction, uint8_t speed)
{
    trainpacket::TrainAddrDecoded trainAddrPacket;
    uint8_t packet[4];
    uint8_t pPos = 1;

    trainAddrPacket = decodeTrainAddr(trainAddr, addrMode);
    if (trainAddrPacket.failure)
    {
        return false;
    }

    if (trainAddrPacket.HighAddrPacket == 0)
    {
        packet[0] = trainAddrPacket.LowAddrPacket;
    }
    else
    {
        packet[0] = trainAddrPacket.HighAddrPacket;
        packet[1] = trainAddrPacket.LowAddrPacket;
        pPos = 2;
    }

    if (direction == TRAIN_SPD_DIR_FOR)
    {
        packet[pPos] = 0x40 + (spd1428Encode(speed, SPD_MODE_28));
    }
    else
    {
        packet[pPos] = 0x60 + (spd1428Encode(speed, SPD_MODE_28));
    }

    // printf("send Speed 28: spd=%d\n", packet[pPos]);
    pPos++;

    dccsignal::calcChecksumPacket(packet, pPos);
    return dccsignal::putPacket(packet, pPos + 1, TRAIN_PACKET_SEND_RETRY_COUNT, trainAddr, 0);
}

bool trainpacket::sendSpeed14Packet(uint16_t trainAddr, uint8_t direction, uint8_t speed)
{
    trainpacket::TrainAddrDecoded trainAddrPacket;
    uint8_t packet[4];
    uint8_t pPos = 1;

    trainAddrPacket = decodeTrainAddr(trainAddr, addrMode);
    if (trainAddrPacket.failure)
    {
        return false;
    }

    if (trainAddrPacket.HighAddrPacket == 0)
    {
        packet[0] = trainAddrPacket.LowAddrPacket;
    }
    else
    {
        packet[0] = trainAddrPacket.HighAddrPacket;
        packet[1] = trainAddrPacket.LowAddrPacket;
        pPos = 2;
    }

    if (direction == TRAIN_SPD_DIR_FOR)
    {
        packet[pPos] = 0x40 + (spd1428Encode(speed, SPD_MODE_14));
    }
    else
    {
        packet[pPos] = 0x60 + (spd1428Encode(speed, SPD_MODE_14));
    }

    // printf("send Speed 14: packet=%d\n", packet[pPos]);
    pPos++;

    dccsignal::calcChecksumPacket(packet, pPos);
    return dccsignal::putPacket(packet, pPos + 1, TRAIN_PACKET_SEND_RETRY_COUNT, trainAddr, 0);
}

bool trainpacket::sendSpeed128Packet(uint16_t trainAddr, uint8_t direction, uint8_t speed)
{
    trainpacket::TrainAddrDecoded trainAddrPacket;
    uint8_t packet[5];
    uint8_t pPos = 1;

    trainAddrPacket = decodeTrainAddr(trainAddr, addrMode);
    if (trainAddrPacket.failure)
    {
        return false;
    }

    if (trainAddrPacket.HighAddrPacket == 0)
    {
        packet[0] = trainAddrPacket.LowAddrPacket;
    }
    else
    {
        packet[0] = trainAddrPacket.HighAddrPacket;
        packet[1] = trainAddrPacket.LowAddrPacket;
        pPos = 2;
    }
    packet[pPos] = 0x3F;
    pPos++;

    if (speed > 127)
    {
        return false;
        //speed = 127;
    }

    if (direction == TRAIN_SPD_DIR_FOR)
    {
        packet[pPos] = 0x80 + speed;
        pPos++;
    }
    else if (direction == TRAIN_SPD_DIR_REV)
    {
        packet[pPos] = speed;
        pPos++;
    }
    else
    {
        return false;
    }

    dccsignal::calcChecksumPacket(packet, pPos);
    return dccsignal::putPacket(packet, pPos + 1, TRAIN_PACKET_SEND_RETRY_COUNT, trainAddr, 0);
}

/*
 *  sendTrainFuncGroupPacket
 *      Send F0-F12 Packet
 *      FuncGroup: Select F0-F4 / F5-F8 / F9-F12
 *      FuncInfo: Function Status Mask
 */
bool trainpacket::sendFuncGroupPacket(uint16_t trainAddr, uint8_t funcGroup, uint8_t funcInfo)
{
    trainpacket::TrainAddrDecoded trainAddrPacket;
    uint8_t packet[4];
    uint8_t pPos = 1;
    uint8_t packetType;

    trainAddrPacket = decodeTrainAddr(trainAddr, addrMode);
    if (trainAddrPacket.failure)
    {
        return false;
    }

    if (trainAddrPacket.HighAddrPacket == 0)
    {
        packet[0] = trainAddrPacket.LowAddrPacket;
    }
    else
    {
        packet[0] = trainAddrPacket.HighAddrPacket;
        packet[1] = trainAddrPacket.LowAddrPacket;
        pPos = 2;
    }

    if (funcGroup == TRAIN_FUNC_BASIC_F0_F4)
    {
        packet[pPos] = 0x80 + (funcInfo & 0x1F);
        packetType = 1;
    }
    else if (funcGroup == TRAIN_FUNC_BASIC_F5_F8)
    {
        packet[pPos] = 0xB0 + (funcInfo & 0x0F);
        packetType = 2;
    }
    else if (funcGroup == TRAIN_FUNC_BASIC_F9_F12)
    {
        packet[pPos] = 0xA0 + (funcInfo & 0x0F);
        packetType = 3;
    }
    else
    {
        return false;
    }
    pPos++;

    dccsignal::calcChecksumPacket(packet, pPos);
    return dccsignal::putPacket(packet, pPos + 1, TRAIN_PACKET_SEND_RETRY_COUNT, trainAddr, packetType);
}

/*
 *  sendTrainExternalFuncPacket
 *      Send F13-F68 Packet
 *      FuncGroup: Select F13-F20 / F21-F28 / F29-F36 / F37-F44 / F45-F52 / F53-F60 / F61-F68
 *      FuncInfo: Function Status Mask
 */
bool trainpacket::sendExternalFuncPacket(uint16_t trainAddr, uint8_t funcGroup, uint8_t funcInfo)
{
    trainpacket::TrainAddrDecoded trainAddrPacket;
    uint8_t packet[5];
    uint8_t pPos = 1;
    uint8_t packetType;

    trainAddrPacket = decodeTrainAddr(trainAddr, addrMode);
    if (trainAddrPacket.failure)
    {
        return false;
    }

    if (trainAddrPacket.HighAddrPacket == 0)
    {
        packet[0] = trainAddrPacket.LowAddrPacket;
    }
    else
    {
        packet[0] = trainAddrPacket.HighAddrPacket;
        packet[1] = trainAddrPacket.LowAddrPacket;
        pPos = 2;
    }

    if (funcGroup == TRAIN_FUNC_EXTERNAL_F13_F20)
    {
        packet[pPos] = 0b11011110;
        packetType = 4;
    }
    else if (funcGroup == TRAIN_FUNC_EXTERNAL_F20_F28)
    {
        packet[pPos] = 0b11011111;
        packetType = 5;
    }
    else if (funcGroup == TRAIN_FUNC_EXTERNAL_F29_F36)
    {
        packet[pPos] = 0b11011000;
        packetType = 6;
    }
    else if (funcGroup == TRAIN_FUNC_EXTERNAL_F37_F44)
    {
        packet[pPos] = 0b11011001;
        packetType = 7;
    }
    else if (funcGroup == TRAIN_FUNC_EXTERNAL_F45_F52)
    {
        packet[pPos] = 0b11011010;
        packetType = 8;
    }
    else if (funcGroup == TRAIN_FUNC_EXTERNAL_F53_F60)
    {
        packet[pPos] = 0b11011011;
        packetType = 9;
    }
    else if (funcGroup == TRAIN_FUNC_EXTERNAL_F61_F68)
    {
        packet[pPos] = 0b11011100;
        packetType = 10;
    }
    else
    {
        return false;
    }
    pPos++;
    packet[pPos] = funcInfo;
    pPos++;

    dccsignal::calcChecksumPacket(packet, pPos);
    return dccsignal::putPacket(packet, pPos + 1, TRAIN_PACKET_SEND_RETRY_COUNT, trainAddr, packetType);
}

uint8_t trainpacket::spd1428Encode(uint8_t spd, uint8_t mode)
{
    if (spd == 0)
    {
        // Stop
        return 0x00;
    }

    if (spd == 1)
    {
        // Emergency Stop
        return 0x01;
    }

    if (mode == SPD_MODE_14)
    {
        // return ((spd & 0x1E) >> 1);
        return ((spd & 0x0F) + 0x10);
    }

    return (((spd & 0x1E) >> 1) + ((spd & 0x01) << 4));
}