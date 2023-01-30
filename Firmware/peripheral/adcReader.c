
#include "pico/stdlib.h"
#include "hardware/adc.h"

#include <stdio.h>

uint8_t adcMsEventCount;

uint16_t adcCache[4];

#define ADC_CURRENT_BUF_COUNT 8
uint16_t adcCurrentBuffer[ADC_CURRENT_BUF_COUNT];
uint8_t adcCurrentBufferIndex;

uint16_t adcPeakCurrent;

void initADCmodule(void)
{
    adc_init();

    adc_gpio_init(26); // Current Sense
    adc_gpio_init(27); // Supply Voltage
    adc_gpio_init(28); // Throttle 1
    adc_gpio_init(29); // Throttle 2

    // hw_set_bits(&adc_hw->div, 8 << 8);
}

uint16_t adcCurrentReadEvent(void)
{
    uint32_t adcSum;
    uint8_t i;

    adc_select_input(0);
    /*
    adcCurrentBuffer[adcCurrentBufferIndex] = adc_read();
    adcPeakCurrent = adcCurrentBuffer[adcCurrentBufferIndex];
    adcCurrentBufferIndex++;
    if (adcCurrentBufferIndex >= ADC_CURRENT_BUF_COUNT)
        adcCurrentBufferIndex = 0;

    for (i = 0; i < ADC_CURRENT_BUF_COUNT; i++)
    {
        adcSum += adcCurrentBuffer[i];
    }
    adcSum = adcSum / 8;

    printf("AV: %d, PV: %d\n", adcSum, adcPeakCurrent);
    return (uint16_t)(adcSum & 0x0000FFFF);
    */
    adcCache[0] = adc_read();
    return adcCache[0];
}

uint16_t adcCurrentPeakRead(void)
{
    adc_select_input(0);
    adcPeakCurrent = adc_read();
    return adcPeakCurrent;
}

uint16_t adcVoltageReadEvent(void)
{
    adc_select_input(1);
    adcCache[1] = adc_read();
    return adcCache[1];
}

void adcMsEvent(void)
{
    if (adcMsEventCount < 2)
    {
        adcMsEventCount = 2;
    }

    if (adcMsEventCount <= 3)
    {
        adc_select_input(adcMsEventCount);
        adcCache[adcMsEventCount] = adc_read();
    }

    adcMsEventCount++;
    if (adcMsEventCount > 3)
    {
        adcMsEventCount = 2;
    }
}

uint16_t readADCcachedValue(uint8_t ch)
{
    if (ch <= 3)
    {
        return adcCache[ch];
    }
    return (0);
}
