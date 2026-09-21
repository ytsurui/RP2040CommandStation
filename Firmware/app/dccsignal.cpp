
#include "pico/stdlib.h"

#include "dccsignal.h"
#include "../peripheral/dccsignalport.h"

#include <stdio.h>
#include "pico/mutex.h"

namespace {
auto_init_mutex(packetBufferMutex);
class PacketBufferLock {
public:
    PacketBufferLock() { mutex_enter_blocking(&packetBufferMutex); }
    ~PacketBufferLock() { mutex_exit(&packetBufferMutex); }
};
}

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

bool dccsignal::cutoutFlagMutexFlag = false;

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
    PacketBufferLock lock;
    return dccPBufCount;
}

// One consumer (core 1) copies a packet while holding the same lock as core 0.
dccsignal::packetStruct *dccsignal::getPacketBuffer(void)
{
    PacketBufferLock lock;
    if (dccPBufCount != 0) {
        dccPacketBuffer &entry = packetBuf[dccPBufOutPos];
        for (uint8_t i = 0; i < entry.length; ++i) {
            sendCache.packet[i] = entry.packet[i];
        }
        sendCache.length = entry.length;
        sendCache.retryCount = entry.retryCount;
        entry.sendWaitFlag = false;
        dccPBufOutPos = (dccPBufOutPos + 1) % PACKETBUFFER_SIZE;
        --dccPBufCount;
        return &sendCache;
    }
    sendCache.packet[0] = 0xFF;
    sendCache.packet[1] = 0x00;
    sendCache.packet[2] = 0xFF;
    sendCache.length = 3;
    sendCache.retryCount = 1;
    return &sendCache;
}

void dccsignal::execPacket(void)
{
    // Release the buffer lock before the relatively slow physical transmission.
    packetStruct packet = *getPacketBuffer();
    for (uint8_t i = 0; i < packet.retryCount; ++i) {
        dccport::dcc_send_packet(packet.packet, packet.length, 1);
        if (dccsignal::checkRunRailcomCutout()) dccport::dcc_railcom_cutout();
    }
}

bool dccsignal::putPacket(uint8_t *packet, uint8_t length, uint8_t cycle, uint16_t targetAddr, uint16_t targetType)
{
    return putPacketImpl(packet, length, cycle, targetAddr, targetType, false);
}

bool dccsignal::putTrainPacket(uint8_t *packet, uint8_t length, uint8_t cycle, uint16_t targetAddr, uint16_t targetType)
{
    return putPacketImpl(packet, length, cycle, targetAddr, targetType, true);
}

bool dccsignal::putPacketImpl(uint8_t *packet, uint8_t length, uint8_t cycle, uint16_t targetAddr, uint16_t targetType, bool replaceable)
{
    if (length == 0 || length > PACKET_MAX_SIZE) return false;
    PacketBufferLock lock;
    // Replace in place even when full. All speed formats share targetType 0.
    if (replaceable) {
        for (uint8_t i = 0; i < PACKETBUFFER_SIZE; ++i) {
            dccPacketBuffer &entry = packetBuf[i];
            if (entry.sendWaitFlag && entry.replaceable &&
                entry.targetAddr == targetAddr && entry.targetType == targetType) {
                for (uint8_t j = 0; j < length; ++j) entry.packet[j] = packet[j];
                entry.length = length;
                entry.retryCount = cycle;
                return true;
            }
        }
    }
    if (dccPBufCount >= PACKETBUFFER_SIZE - 3) return false;
    dccPacketBuffer &entry = packetBuf[dccPBufInPos];
    for (uint8_t i = 0; i < length; ++i) entry.packet[i] = packet[i];
    entry.length = length;
    entry.retryCount = cycle;
    entry.targetAddr = targetAddr;
    entry.targetType = targetType;
    entry.replaceable = replaceable;
    entry.sendWaitFlag = true;
    dccPBufInPos = (dccPBufInPos + 1) % PACKETBUFFER_SIZE;
    ++dccPBufCount;
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