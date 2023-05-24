
#include "pico/stdlib.h"
#include "pico/multicore.h"

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

void subCoreMain();

// uint16_t counter;

// uint16_t counter2, counter3;

void sendWiredUart(uint8_t data)
{
    uartCtrl::getInstance(0)->send(data);
    printf("data: %c\n", data);
    //uartCtrl::getInstance(1)->send(data);
}

void sendWirelessUart(uint8_t data)
{
    uartCtrl::getInstance(1)->send(data);
}


void wirelessRecv(uint8_t data)
{
    // Echo
    uartCtrl::getInstance(1)->send(data);
    loconetPacketRouter::recv(data);
}

void wiredRecv(uint8_t data)
{
    //loconetPacketRouter::recv(data);
    //printf("recv: %c\n", data);
    mt40busCtrl::recv(data);
}

//  Main Core Routine
int main()
{
    uint8_t adcEventCounter = 0;

    // uartTest::init();
    uartCtrl::globalInit();

    //uartCtrl::getInstance(0)->setBaudRate(16600);
    uartCtrl::getInstance(0)->setBaudRate(115200);
    uartCtrl::getInstance(1)->setBaudRate(19200);

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

    loconetPacketRouter::init();
    // uartTest::setCallback(loconetPacketRouter::recv);
    // loconetPacketRouter::setSender(uartTest::send);

    // uartCtrl::getInstance(0)->setRecvCallback(loconetPacketRouter::recv);
    uartCtrl::getInstance(0)->setRecvCallback(wiredRecv);
    uartCtrl::getInstance(1)->setRecvCallback(wirelessRecv);
    //loconetPacketRouter::setSender(sendWiredUart);
    loconetPacketRouter::setSender(sendWirelessUart);

    mt40busCtrl::setSender(sendWiredUart);

    currentMonitor::init();
    voltageMonitor::init();

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
                // toggleDCCpowerStat();
                dccport::togglePowerStat();
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