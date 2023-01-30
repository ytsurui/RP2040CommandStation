
#include "pico/stdlib.h"
#include "throttle.h"

#include "../peripheral/adcReader.h"
#include "trainpacket.h"
#include "../throttle/peripheral/direction.h"
#include "trainctrl.h"

uint8_t throttleApp::throttle1Value;
uint8_t throttleApp::throttle2Value;

uint8_t throttleApp::throttle1LastSend;
uint8_t throttleApp::throttle2LastSend;
#define LAST_SEND_INTERVAL_SPD 50

void throttleApp::event(void)
{

    uint8_t i, volumeValue;
    trainctrl::trainctrlresp trainCtrlObj;

    volumeValue = readADCcachedValue(3) >> 5;
    if (volumeValue != throttle1Value)
    {
        throttle1Value = volumeValue;
        trainCtrlObj = trainctrl::getTrainCtrl(3);
        //if (trainCtrlObj.enable)
        //{
        //trainCtrlObj.train.setSpeed128(directionSW::readStat(0), volumeValue);
        trainCtrlObj.train->setSpeed128(directionSW::readStat(0), volumeValue);
        //}
    }

    volumeValue = readADCcachedValue(2) >> 5;
    if (volumeValue != throttle2Value)
    {
        throttle2Value = volumeValue;
        trainCtrlObj = trainctrl::getTrainCtrl(4);
        //if (trainCtrlObj.enable)
        //{
        //trainCtrlObj.train.setSpeed128(directionSW::readStat(1), volumeValue);
        if (trainCtrlObj.train != nullptr)
        {
            trainCtrlObj.train->setSpeed128(directionSW::readStat(1), volumeValue);
        }
        //}
    }

    /*
    if (throttle1LastSend == 0)
    {
        volumeValue = readADCcachedValue(3) >> 5;
        if (volumeValue != throttle1Value)
        {
            throttle1Value = volumeValue;
            //if (trainpacket::sendSpeed128Packet(3, directionSW::readStat(0), throttle1Value))
            //{
            //    throttle1LastSend = LAST_SEND_INTERVAL_SPD;
            //}

            throttle1LastSend = LAST_SEND_INTERVAL_SPD;
        }
    }
    else
    {
        throttle1LastSend--;
    }

    if (throttle2LastSend == 0)
    {
        volumeValue = readADCcachedValue(2) >> 5;
        if (volumeValue != throttle2Value)
        {
            throttle2Value = volumeValue;
            if (trainpacket::sendSpeed128Packet(4, directionSW::readStat(1), throttle2Value))
            {
                throttle2LastSend = LAST_SEND_INTERVAL_SPD;
            }
        }
    }
    else
    {
        throttle2LastSend--;
    }
    */
}
