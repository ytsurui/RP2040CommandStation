
#include "pico/stdlib.h"
#include "voltage_monitor.h"

#include "../peripheral/adcReader.h"
#include "../peripheral/dccsignalport.h"
#include "../peripheral/fail_led.h"

#include <stdio.h>

uint16_t voltageMonitor::counter;
uint16_t voltageMonitor::mv;
uint16_t voltageMonitor::mvInit;

void voltageMonitor::init(void)
{
    uint8_t i;
    // uint16_t voltBuf[32];
    uint16_t voltBuf;
    uint32_t voltSum = 0;

    sleep_ms(100);

    for (i = 0; i < 32; i++)
    {
        voltBuf = (adcVoltageReadEvent() * 9);
        voltSum += voltBuf;
        //printf("voltBuf[%d]=%d, voltSum=%d\n", i, voltBuf, voltSum);
        sleep_ms(5);
    }

    mvInit = (uint16_t)(voltSum / 32);

    //printf("Voltage Initial: %d\n", mvInit);
}

void voltageMonitor::event(void)
{
    // if (dccport::dcc_railcom_cutout_running())
    //{
    //     return;
    // }
    // return;

    counter++;
    if (counter >= 5)
    {
        counter = 0;
        mv = (adcVoltageReadEvent() * 9);

        if (mvInit > mv)
        {
            if ((mvInit - mv) > 3000)
            {
                printf("Voltage Down Emergency Shutdown, initvalue=%d, currentVoltage=%d\n", mvInit, mv);
                dccport::setPowerStat(false);
                failureLED::setStat(true);
            }
        }

        /*
        if (mvInit > mv && (mvInit - mv) > 1000)
        {
            // Emergency Shutdown
            printf("Voltage Down Emergency Shutdown, initvalue=%d, currentVoltage=%d\n", mvInit, mv);
            dccport::setPowerStat(false);
            failureLED::setStat(true);
        }
        */
    }
}

uint16_t voltageMonitor::get(void)
{
    return mv;
}