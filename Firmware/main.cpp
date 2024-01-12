
#include "pico/stdlib.h"
#include "pico/multicore.h"

#include "class/cdc/cdc_device.h"

#include "peripheral/eventTimer.h"
#include "peripheral/adcReader.h"
#include "peripheral/dccsignalport.h"
#include "peripheral/i2c_ctrl.h"
#include "peripheral/powerButton.h"
#include "peripheral/fail_led.h"
#include "peripheral/led_comm.h"

#include "app/throttle/th_main.h"

// #include "throttle/peripheral/lcd.h"
// #include "throttle/peripheral/keypad.h"
// #include "throttle/peripheral/direction.h"

#include "app/dccsignal.h"
#include "app/current_monitor.h"
#include "app/voltage_monitor.h"
// #include "app/throttle.h"

#include "app/trainctrl.h"
#include "app/acc_ctrl.h"

#include "app/progtrak.h"

// #include "peripheral/uarttest.h"
#include "peripheral/uart_ctrl.h"
#include "app/loconet/packetRouter.h"

#include "app/mt40bus/mt40bus.h"

#include <stdio.h>

#ifdef ENABLE_LBUS
//#define WIRELESS_MODE_LBUS
#endif

void subCoreMain();

// uint16_t counter;

// uint16_t counter2, counter3;

void sendWiredUart(uint8_t data)
{
    commLED::set();
    uartCtrl::getInstance(0)->send(data);
    //printf("data: %c\n", data);
    //uartCtrl::getInstance(1)->send(data);
    //printf("%c", data);
}

void sendMT40BUSdata(uint8_t data)
{
    commLED::set();
    uartCtrl::getInstance(0)->send(data);

#ifndef WIRELESS_MODE_LBUS
    uartCtrl::getInstance(1)->send(data);
#endif
    printf("%c", data);
}

void sendWirelessUart(uint8_t data)
{
    uartCtrl::getInstance(1)->send(data);
}


void wirelessRecv(uint8_t data)
{
#ifdef WIRELESS_MODE_LBUS
    // Echo
    uartCtrl::getInstance(1)->send(data);
    loconetPacketRouter::recv(data);
#else
    mt40busCtrl::recvObj[1].recv(data);
#endif
}

void wiredRecv(uint8_t data)
{
    //loconetPacketRouter::recv(data);
    //printf("recv: %c\n", data);
    commLED::set();
    //mt40busCtrl::recv(data);
    mt40busCtrl::recvObj[0].recv(data);
}

void wiredRecvToUSBecho(uint8_t data)
{
    printf("%c", data);
}

void wiredRecvEchoOtherPort(uint8_t data, bool privatePacket)
{
    printf("%c", data);
#ifndef WIRELESS_MODE_LBUS
    // Wired to Wireless bridge
    if (!privatePacket) {
        uartCtrl::getInstance(1)->send(data);
    }
#endif
}

void wirelessRecvEchoOtherPort(uint8_t data, bool privatePacket)
{
    printf("%c", data);
    if (!privatePacket) {
        uartCtrl::getInstance(0)->send(data);
    }
}

void usbRecvEchoOtherPort(uint8_t data, bool privatePacket)
{
    if (!privatePacket) {
        uartCtrl::getInstance(0)->send(data);
#ifndef WIRELESS_MODE_LBUS
        // Wired to Wireless bridge
        uartCtrl::getInstance(1)->send(data);
#endif
    }
}

void recvUSBserialChar(void)
{
    uint8_t rData;
    rData = tud_cdc_read_char();
    printf("%c", rData);
    mt40busCtrl::recvObj[2].recv(rData);
}

bool wiredCarrierSense(void)
{
    return uartCtrl::getBusGPIO(1);
}

#ifdef ENABLE_LBUS

void sendLBUScommand(uint8_t *buf, uint8_t length)
{
    uint8_t i;

    //printf("sendLBUScommand called: data=%s, length=%d\n", buf, length);

    mt40busCtrl::sendLBUSdata(buf, length);

#ifdef WIRELESS_MODE_LBUS
    for (i = 0; i < length; i++) {
        sendWirelessUart(buf[i]);
    }
#endif

}

#endif

//  Main Core Routine
int main()
{
    uint8_t adcEventCounter = 0;
    uint8_t bootEventID;
    uint32_t bootEventCount;

    // uartTest::init();
    uartCtrl::globalInit();

    //uartCtrl::getInstance(0)->setBaudRate(16600);
    uartCtrl::getInstance(0)->setBaudRate(115200);

#ifdef WIRELESS_MODE_LBUS
    uartCtrl::getInstance(1)->setBaudRate(19200);
#else
    uartCtrl::getInstance(1)->setBaudRate(115200);
#endif

    stdio_init_all();

    gpio_init(6);
    gpio_set_dir(6, GPIO_OUT);
    gpio_put(6, true);

    gpio_init(7);
    gpio_set_dir(7, GPIO_OUT);
    gpio_put(7, false);

    // initDCCsignalPort();
    dccport::init();
    initADCmodule();
    initI2Cport();
    powerbutton::init();
    failureLED::init();
    commLED::init();

    // keypad::init();
    // directionSW::init();

    trainctrl::init();
    accessoryCtrl::init();

    // initLCD();
    throttleAppMain::init();

    eventtimer::init();

    throttleAppMain::bootVersionInfo();
    bootEventCount = 0;
    
    while(1) {
        if (eventtimer::checkMS()) {
            bootEventID = throttleAppMain::bootMsgShowTimming();
            if (bootEventID == 0xFF) {
                break;
            }

            if (bootEventID == 1) {
                throttleAppMain::bootVersionInfo();
            }
            if (bootEventCount <= 2) {
                bootEventCount++;
            }

            if (bootEventCount == 1) {
                loconetPacketRouter::init();

                uartCtrl::getInstance(0)->setRecvCallback(wiredRecv);
                uartCtrl::getInstance(1)->setRecvCallback(wirelessRecv);

#ifdef ENABLE_LBUS
                loconetPacketRouter::setMultidataSender(sendLBUScommand);
                mt40busCtrl::setLBUScommandReceiver(loconetPacketRouter::recv);
#endif

                //mt40busCtrl::setSender(sendWiredUart);
                mt40busCtrl::setSender(sendMT40BUSdata);
                mt40busCtrl::setCarrierSenseFunc(wiredCarrierSense);

                mt40busCtrl::recvObj[0].receivedEchoCb(wiredRecvEchoOtherPort);
                mt40busCtrl::recvObj[1].receivedEchoCb(wirelessRecvEchoOtherPort);
                mt40busCtrl::recvObj[2].receivedEchoCb(usbRecvEchoOtherPort);

                currentMonitor::init();
                voltageMonitor::init();
            }
        }
    }

//    loconetPacketRouter::init();
    // uartTest::setCallback(loconetPacketRouter::recv);
    // loconetPacketRouter::setSender(uartTest::send);

    // uartCtrl::getInstance(0)->setRecvCallback(loconetPacketRouter::recv);
//    uartCtrl::getInstance(0)->setRecvCallback(wiredRecv);
//    uartCtrl::getInstance(1)->setRecvCallback(wirelessRecv);
    //loconetPacketRouter::setSender(sendWiredUart);
    //loconetPacketRouter::setSender(sendWirelessUart);

//#ifdef ENABLE_LBUS
//    loconetPacketRouter::setMultidataSender(sendLBUScommand);
//    mt40busCtrl::setLBUScommandReceiver(loconetPacketRouter::recv);
//#endif

//    mt40busCtrl::setSender(sendWiredUart);
//    mt40busCtrl::setCarrierSenseFunc(wiredCarrierSense);

//    mt40busCtrl::recvObj[0].receivedEchoCb(wiredRecvEchoOtherPort);
//    mt40busCtrl::recvObj[1].receivedEchoCb(wirelessRecvEchoOtherPort);
//    mt40busCtrl::recvObj[2].receivedEchoCb(usbRecvEchoOtherPort);

//    currentMonitor::init();
//    voltageMonitor::init();

    multicore_launch_core1(subCoreMain);

    // uint16_t counter;

    // testMessage();

    // trainctrl::selectNewTrain(3);
    // trainctrl::selectNewTrain(4);

    while (1)
    {

        if (progtrak::readCurrentRunFlag())
        {
            // adcCurrentReadEvent();
            progtrak::readCurrent();
        }

        if (tud_cdc_available()) {
            recvUSBserialChar();
        }

        // currentMonitor::task();

        if (eventtimer::checkMS())
        {
            // 1ms Event
            // eventDCCsignalGenerater();
            dccsignal::event();

            if (adcEventCounter == 0)
            {
                currentMonitor::event();
                // adcEventCounter++;
            }
            else if (adcEventCounter == 1)
            {
                voltageMonitor::event();
                // adcEventCounter = 0;
            }
            else if (adcEventCounter == 2 || adcEventCounter == 3)
            {
                adcMsEvent();
            }

            adcEventCounter++;
            if (adcEventCounter >= 4)
            {
                adcEventCounter = 0;
            }

            dccport::event();

            powerbutton::event();
            // keypad::event();
            // directionSW::event();
            failureLED::event();
            commLED::event();

            throttleAppMain::event();

            // adcMsEvent();

            // eventThrottleApp();
            trainctrl::eventMS();

            // throttleApp::event();

            loconetPacketRouter::event();

            mt40busCtrl::eventMS();
        }

        trainctrl::task();
        accessoryCtrl::task();
        // uartTest::task();
        uartCtrl::globalTask();

        throttleAppMain::task();

        // if (checkChangePowerButtonStat())
        if (powerbutton::checkChange())
        {
            // setDCCpowerStat(readPowerButtonStat());
            // if (readPowerButtonStat())
            if (powerbutton::readStat())
            {
                if (mt40busCtrl::getPowerManagerDown()) {
                    mt40busCtrl::sendCmdPMUP(0);
                    mt40busCtrl::clearPMdownStat(0);
                    dccport::setPowerStat(true);
                } else {
                    // toggleDCCpowerStat();
                    dccport::togglePowerStat();
                }
                mt40busCtrl::sendCmdPWSresp();
            }
        }
    }

    // puts("Hello, world!");

    return 0;
}

//  Sub Core Routine (DCC-Signal Generating)
void subCoreMain()
{
    while (1)
    {
        progtrak::EventProg();

        // if (checkRunRailcomCutout())
        if (dccsignal::checkRunRailcomCutout())
        {
            // dcc_railcom_cutout();
            // printf("BiDi cutout\n");
            dccport::dcc_railcom_cutout();
        }

        // execDCCpacket();
        // printf("exec Packet\n");
        dccsignal::execPacket();
        // dcc_send_preamble();
    }
}