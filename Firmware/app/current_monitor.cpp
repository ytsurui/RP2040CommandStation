#include "pico/stdlib.h"
#include "current_monitor.h"

#include "../peripheral/adcReader.h"
#include "../peripheral/dccsignalport.h"
#include "../peripheral/fail_led.h"
#include "../throttle/peripheral/lcd.h"

#include <stdio.h>

uint8_t currentMonitor::counter = 0;
uint16_t currentMonitor::currentValue = 0;
uint16_t currentMonitor::overCurrentCounter = 0;
uint16_t currentMonitor::overCurrentReturnCheckCounter = 0;
uint16_t currentMonitor::currentAdcOffsetValue;

uint16_t currentMonitor::viewCurrentValue = 0;
uint16_t currentMonitor::currValueArray[CURR_VALUE_ARRAY_LENGTH];
uint8_t currentMonitor::currValueIndex = 0;

//#define SHUTDOWN_CURRENT 1600
#define SHUTDOWN_CURRENT 4500 // High-Power
#define EMER_CURRENT SHUTDOWN_CURRENT * 2.5

#define OVERCURRENT_COUNTER_MAX 200
#define OVERCURRENT_RETURN_VALUE 100

void currentMonitor::init(void)
{
    uint16_t currTank[32];
    uint32_t currSum;
    uint8_t i;

    sleep_ms(100);

    for (i = 0; i < 32; i++)
    {
        currTank[i] = adcCurrentPeakRead();
        sleep_ms(5);
    }

    currSum = 0;

    for (i = 0; i < 32; i++)
    {
        currSum += currTank[i];
        printf("currTank[%d]=%d, currSum=%d\n", i, currTank[i], currSum);
    }

    printf("currSum: %d\n", currSum);

    currentAdcOffsetValue = (uint16_t)(currSum / 32);

    printf("current ADC offset: %d\n", currentAdcOffsetValue);
}

void currentMonitor::task(void)
{
    uint16_t peakCurrTmp;

    // if (dccport::dcc_railcom_cutout_running())
    //{
    //     return;
    // }

    /*
    peakCurrTmp = adcCurrentPeakRead();
    if (peakCurrTmp < currentAdcOffsetValue)
    {
        peakCurrTmp = 0;
    }
    else
    {
        peakCurrTmp -= currentAdcOffsetValue;
    }
    peakCurrTmp *= 3.0;

    if (peakCurrTmp > EMER_CURRENT)
    {
        printf("emergency shutdown, value=%d\n", peakCurrTmp);
        dccport::setPowerStat(false);
        failureLED::setStat(true);
    }
    */
}

void currentMonitor::event(void)
{
    uint16_t adcReadValue;
    uint32_t currSum;
    uint8_t i;

    // if (dcc_railcom_cutout_running())
    if (dccport::dcc_railcom_cutout_running())
    {
        return;
    }

    if (overCurrentCounter > 0)
    {
        overCurrentCounter++;
    }
    // counter++;
    // if (counter >= 5)
    //{
    //     counter = 0;
    adcReadValue = adcCurrentReadEvent();
    if (adcReadValue < currentAdcOffsetValue)
    {
        adcReadValue = 0;
    }
    else
    {
        adcReadValue -= currentAdcOffsetValue;
    }
    currentValue = adcReadValue * 3.0;
    /*
    if (currentValue > EMER_CURRENT)
    {
        dccport::setPowerStat(false);
        failureLED::setStat(true);
    }
    else if (currentValue > SHUTDOWN_CURRENT)
    {
        if (overCurrentCounter == 1)
        {
            dccport::setPowerStat(false);
            failureLED::setStat(true);
            overCurrentCounter = 0;
        }
        else
        {
            overCurrentCounter = 1;
        }
    }
    else
    {
        if (overCurrentCounter != 0)
        {
            overCurrentCounter = 0;
        }
    }
    */
    if (dccport::powerInitCheck())
    {
        return;
    }

    if (currentValue > SHUTDOWN_CURRENT)
    {
        if (overCurrentCounter == 0)
        {
            overCurrentCounter = 1;
            overCurrentReturnCheckCounter = 0;
        }
        else
        {
            if (currentValue > EMER_CURRENT)
            {
                overCurrentCounter++;
            }

            if (overCurrentCounter >= OVERCURRENT_COUNTER_MAX)
            {
                printf("overcurrent shutdown, value=%d\n", currentValue);
                dccport::setPowerStat(false);
                failureLED::setStat(true);
            }
            else if ((OVERCURRENT_COUNTER_MAX - overCurrentCounter) < OVERCURRENT_RETURN_VALUE)
            {
                printf("overcurrent timeout shutdown, value=%d\n", currentValue);
                dccport::setPowerStat(false);
                failureLED::setStat(true);
            }
        }
        // printf("overcurrent shutdown, value=%d\n", currentValue);
        // dccport::setPowerStat(false);
        // failureLED::setStat(true);
    }
    else
    {
        if (overCurrentCounter > 0)
        {
            if (currentValue <= SHUTDOWN_CURRENT)
            {
                overCurrentReturnCheckCounter++;
                if (overCurrentReturnCheckCounter >= OVERCURRENT_RETURN_VALUE)
                {
                    overCurrentReturnCheckCounter = 0;
                    overCurrentCounter = 0;
                }
            }
            else
            {
                overCurrentReturnCheckCounter = 0;
            }
        }
    }

    currValueArray[currValueIndex] = currentValue;
    currValueIndex++;
    if (currValueIndex >= CURR_VALUE_ARRAY_LENGTH)
    {
        currValueIndex = 0;
    }

    currSum = 0;
    for (i = 0; i < CURR_VALUE_ARRAY_LENGTH; i++)
    {
        currSum = currSum + currValueArray[i];
    }

    viewCurrentValue = (currSum / CURR_VALUE_ARRAY_LENGTH);
    // printf("c:%d, v:%d\n", currentValue, viewCurrentValue);

    /*
    peakCurrTmp > adcCurrentPeakRead() * 3.0;
    if (peakCurrTmp > EMER_CURRENT)
    {
        dccport::setPowerStat(false);
        failureLED::setStat(true);
    }
    */
    //}
}

void currentMonitor::print(void)
{
    setLCDpos(0, 0);
    sendSpaceLCD(4);
    setLCDpos(0, 0);
    sendNum(currentValue);
    writeLCDdata('m');
    writeLCDdata('A');
}

uint16_t currentMonitor::getCurrent(void)
{
    printf("currentValue: %d\n", currentValue);
    return (currentValue);
}