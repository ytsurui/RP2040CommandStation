
#include "pico/stdlib.h"

#include "dccsignal.h"
#include "../peripheral/dccsignalport.h"

#include <stdio.h>

#define CUTOUT_TIMMING_COUNT_MAX 10
#define CUTOUT_TIMMING_COUNT_LONG_MAX 10
static int railcomCutoutTimmingCounter = 0;
static bool railcomCutoutLongMode;
static bool railcomCutoutFlag = false;

dccsignal::dccPacketBuffer dccsignal::packetBuf[PACKETBUFFER_SIZE];

uint8_t dccsignal::dccPBufInPos = 0;
uint8_t dccsignal::dccPBufOutPos = 0;
uint8_t dccsignal::dccPBufCount = 0;

bool dccsignal::enableBiDiCutout = true;

bool dccsignal::bufferMutexFlag = false;
bool dccsignal::cutoutFlagMutexFlag = false;
bool dccsignal::bufferCounterMutexFlag = false;

dccsignal::packetStruct dccsignal::sendCache;

// void eventDCCsignalGenerater(void)
void dccsignal::event(void)
{
    int count_max;
    if (railcomCutoutLongMode)
    {
        count_max = CUTOUT_TIMMING_COUNT_LONG_MAX;
    }
    else
    {
        count_max = CUTOUT_TIMMING_COUNT_MAX;
    }

    railcomCutoutTimmingCounter++;
    if (!enableBiDiCutout) {
        railcomCutoutTimmingCounter = 0;
    }

    // if (railcomCutoutTimmingCounter > CUTOUT_TIMMING_COUNT_MAX)
    if (railcomCutoutTimmingCounter > count_max)
    {
        while (cutoutFlagMutexFlag)
            ;
        cutoutFlagMutexFlag = true;
        railcomCutoutFlag = true;
        cutoutFlagMutexFlag = false;
        railcomCutoutTimmingCounter = 0;
        railcomCutoutLongMode = !railcomCutoutLongMode;
    }
}

// bool checkRunRailcomCutout(void)
bool dccsignal::checkRunRailcomCutout(void)
{
    while (cutoutFlagMutexFlag)
        ;

    cutoutFlagMutexFlag = true;
    if (railcomCutoutFlag)
    {
        railcomCutoutFlag = false;
        cutoutFlagMutexFlag = false;
        return true;
    }
    cutoutFlagMutexFlag = false;
    return false;
}

// パケットバッファをスキャンし、送信待ちになっているパケット数をカウントする
uint8_t dccsignal::getWaitPacketCount(void)
{
    uint8_t i, count;

    count = 0;
    for (i = 0; i < PACKETBUFFER_SIZE; i++)
    {
        if (packetBuf[i].sendWaitFlag)
        {
            count++;
        }
    }
    return (count);
}

// パケットバッファをスキャンし、出力するパケットデータのアドレスを返す
// packetStruct *getPacketBuffer(void)
dccsignal::packetStruct *dccsignal::getPacketBuffer(void)
{
    uint8_t i, i2;
    uint8_t pIndex;

    for (i = 0; i < PACKETBUFFER_SIZE; i++)
    {
        pIndex = i + dccPBufOutPos;
        if (pIndex >= PACKETBUFFER_SIZE)
        {
            pIndex -= PACKETBUFFER_SIZE;
        }

        if (pIndex >= PACKETBUFFER_SIZE)
        {
            continue;
        }

        if (packetBuf[pIndex].sendWaitFlag)
        {
            for (i2 = 0; i2 < packetBuf[pIndex].length; i2++)
            {
                sendCache.packet[i2] = packetBuf[pIndex].packet[i2];
            }

            sendCache.length = packetBuf[pIndex].length;
            sendCache.retryCount = packetBuf[pIndex].retryCount;
            packetBuf[pIndex].sendWaitFlag = false;

            while (bufferCounterMutexFlag)
                ;
            bufferCounterMutexFlag = true;
            dccPBufOutPos++;
            if (dccPBufCount > 0)
            {
                dccPBufCount--;
            }

            if (dccPBufOutPos >= PACKETBUFFER_SIZE)
            {
                dccPBufOutPos = 0;
            }
            bufferCounterMutexFlag = false;

            // printf("send packet cache index: %d, count=%d\n", pIndex, dccPBufCount);
            return &sendCache;
        }
    }

    if (dccPBufCount > 0)
    {
        // printf("error-correct: dccPBufCount Invalid, value=%d, set zero\n", dccPBufCount);
        dccPBufCount = 0;
    }

    // printf("send idle packet\n");

    // 送信キャッシュが空の場合はアイドルパケットを送信する
    sendCache.packet[0] = 0xFF;
    sendCache.packet[1] = 0x00;
    sendCache.packet[2] = 0xFF;
    sendCache.length = 3;
    sendCache.retryCount = 1;
    return &sendCache;
}

// void execDCCpacket(void)
void dccsignal::execPacket(void)
{
    packetStruct *sendData;
    packetStruct cpBuf;
    uint8_t i;

    sendData = getPacketBuffer();

    while (bufferMutexFlag)
        ;
    bufferMutexFlag = true;
    for (i = 0; i < sendData->length; i++)
    {
        cpBuf.packet[i] = sendData->packet[i];
    }
    cpBuf.length = sendData->length;
    cpBuf.retryCount = sendData->retryCount;
    bufferMutexFlag = false;

    // dccport::dcc_send_packet(sendData->packet, sendData->length, sendData->retryCount);
    for (i = 0; i < cpBuf.retryCount; i++)
    {
        dccport::dcc_send_packet(cpBuf.packet, cpBuf.length, 1);
        if (dccsignal::checkRunRailcomCutout())
        {
            dccport::dcc_railcom_cutout();
        }
    }
}

// bool putPacket(uint8_t *packet, uint8_t length, uint8_t cycle, uint16_t targetAddr, uint16_t targetType)
bool dccsignal::putPacket(uint8_t *packet, uint8_t length, uint8_t cycle, uint16_t targetAddr, uint16_t targetType)
{
    uint8_t i, pIndex;

    if (dccPBufCount >= (PACKETBUFFER_SIZE - 3))
    {
        // printf("putPacket error: buffer overflow, bufCount=%d, bufPos=%d\n", dccPBufCount, dccPBufInPos);
        return false;
    }

    while (bufferCounterMutexFlag)
        ;
    if (dccPBufInPos >= PACKETBUFFER_SIZE)
    {
        bufferCounterMutexFlag = true;
        dccPBufInPos = 0;
        bufferCounterMutexFlag = false;
    }

    while (bufferMutexFlag)
        ;

    bufferMutexFlag = true;

    if (packetBuf[dccPBufInPos].sendWaitFlag)
    {
        bufferMutexFlag = false;
        // printf("putPacket error: buffer send wait: index=%d\n", dccPBufInPos);
        return false;
    }

    for (i = 0; i < length; i++)
    {
        packetBuf[dccPBufInPos].packet[i] = packet[i];
    }
    packetBuf[dccPBufInPos].length = length;
    packetBuf[dccPBufInPos].retryCount = cycle;
    packetBuf[dccPBufInPos].sendWaitFlag = true;
    packetBuf[dccPBufInPos].targetAddr = targetAddr;
    packetBuf[dccPBufInPos].targetType = targetType;

    while (bufferCounterMutexFlag)
        ;
    bufferCounterMutexFlag = true;
    dccPBufInPos++;
    dccPBufCount++;
    if (dccPBufInPos >= PACKETBUFFER_SIZE)
    {
        dccPBufInPos = 0;
    }
    bufferCounterMutexFlag = false;
    bufferMutexFlag = false;
    return true;
}

// void calcChecksumPacket(uint8_t *packet, uint8_t length)
void dccsignal::calcChecksumPacket(uint8_t *packet, uint8_t length)
{
    uint8_t i;

    packet[length] = 0;
    for (i = 0; i < length; i++)
    {
        packet[length] ^= packet[i];
    }
}

void dccsignal::SetBiDiCutout(bool newstat)
{
    enableBiDiCutout = newstat;
}

bool dccsignal::GetBiDiCutoutStat(void)
{
    return enableBiDiCutout;
}