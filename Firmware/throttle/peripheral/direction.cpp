
#include "pico/stdlib.h"
#include "direction.h"

#define DIR_SCAN_SEL_1_PORT 22
#define DIR_SCAN_SEL_2_PORT 23
#define DIR_SCAN_IN_1_PORT 20
#define DIR_SCAN_IN_2_PORT 21

#define DIR_SEL_PORT_MASK (1 << DIR_SCAN_SEL_1_PORT) | (1 << DIR_SCAN_SEL_2_PORT)
#define DIR_IN_PORT_MASK (1 << DIR_SCAN_IN_1_PORT) | (1 << DIR_SCAN_IN_2_PORT)

uint8_t directionReadCounter;
//uint8_t directionData;
//uint8_t directionReadData;
//uint8_t directionChangeFlag;

uint8_t directionData[2];

void directionSW::init(void)
{
    gpio_init_mask(DIR_SEL_PORT_MASK | DIR_IN_PORT_MASK);
    gpio_set_dir_in_masked(DIR_IN_PORT_MASK);
    gpio_pull_down(DIR_SCAN_IN_1_PORT);
    gpio_pull_down(DIR_SCAN_IN_2_PORT);
    gpio_set_dir_in_masked(DIR_SEL_PORT_MASK);
    gpio_pull_down(DIR_SCAN_SEL_1_PORT);
    gpio_pull_down(DIR_SCAN_SEL_2_PORT);
    directionReadCounter = 0xFF;
}

void directionSW::event(void)
{
    uint8_t gpioRead;
    uint8_t gpioStat;

    directionReadCounter++;
    if (directionReadCounter > 3)
    {
        //directionReadCounter = 0xFF;
        //return;
        directionReadCounter = 0;
    }

    if (~directionReadCounter & 0x01)
    {
        // Set Port

        switch (directionReadCounter & 0xFE)
        {
        case 0x00:
            gpio_set_dir(DIR_SCAN_SEL_1_PORT, GPIO_OUT);
            gpio_put(DIR_SCAN_SEL_1_PORT, true); // TH1-DIR
            break;
        case 0x02:
            gpio_set_dir(DIR_SCAN_SEL_2_PORT, GPIO_OUT);
            gpio_put(DIR_SCAN_SEL_2_PORT, true); // TH2-DIR
            break;
        default:
            directionReadCounter = 0xFF;
            break;
        }
    }
    else
    {
        // Read
        if (gpio_get(DIR_SCAN_IN_1_PORT))
        {
            // IN1 - REV
            gpioStat = 0x02;
        }
        else if (gpio_get(DIR_SCAN_IN_2_PORT))
        {
            // IN2 - FOR
            gpioStat = 0x01;
        }
        else
        {
            gpioStat = 0x00;
        }

        switch (directionReadCounter & 0xFE)
        {
        case 0x00:
            // Throttle1
            if (gpioStat != (directionData[0] & 0x03))
            {
                directionData[0] = gpioStat | 0x80;
            }
            break;
        case 0x02:
            // Throttle2
            if (gpioStat != (directionData[1] & 0x03))
            {
                directionData[1] = gpioStat | 0x80;
            }
            break;
        }

        gpio_clr_mask(DIR_SEL_PORT_MASK);
        gpio_set_dir_in_masked(DIR_SEL_PORT_MASK);
    }
}

bool directionSW::checkChange(uint8_t channel)
{
    if (channel >= 2)
        return false;

    if (directionData[channel] & 0x80)
    {
        directionData[channel] &= ~0x80;
        return true;
    }
    return false;
}

uint8_t directionSW::readStat(uint8_t channel)
{
    if (channel >= 2)
        return (0);

    return (directionData[channel] & 0x03);
}
