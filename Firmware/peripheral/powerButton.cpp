
#include "pico/stdlib.h"
#include "powerButton.h"

#define POWER_BUTTON_PORT 25
#define POWER_BUTTON_COUNTER_THRESHOLD 2
#define POWER_BUTTON_COUNTER_MAX 16

uint8_t powerbutton::counter = 0;
uint8_t powerbutton::flag = 0;

// void initPowerButton(void)
void powerbutton::init()
{
    gpio_init(POWER_BUTTON_PORT);
    gpio_set_dir(POWER_BUTTON_PORT, false);
    gpio_pull_up(POWER_BUTTON_PORT);
    // counter = POWER_BUTTON_COUNTER_MAX;
    //counter = 0;
}

// void eventPowerButton(void)
void powerbutton::event()
{
    if (gpio_get(POWER_BUTTON_PORT)) {
        if (counter > 0) {
            counter--;

            if (counter <= 0) {
                if (flag & 0x40) {
                    flag &= ~0x40;
                    flag |= 0x80;
                }
            }
        }
    } else {
       
        if (counter < POWER_BUTTON_COUNTER_MAX) {
            counter++;
            if (counter >= POWER_BUTTON_COUNTER_MAX) {
                if (~flag & 0x40) {
                    flag |= 0xC0;
                }
            }
        }
        
    }
    
    /*
    if (gpio_get(POWER_BUTTON_PORT))
    {
        if ((counter & 0x6F) < POWER_BUTTON_COUNTER_MAX)
        {
            counter++;

            if ((counter & 0x6F) > POWER_BUTTON_COUNTER_THRESHOLD)
            {
                counter |= 0xC0;
            }
        }
    }
    else
    {
        if ((counter & 0x6F) > 0)
        {
            counter--;
            if ((counter & 0x6F) == 0)
            {
                counter &= ~0x40;
                counter |= 0x80;
            }
        }
    }
    */
}

// bool checkChangePowerButtonStat(void)
bool powerbutton::checkChange(void)
{
    if (flag & 0x80)
    {
        flag &= ~0x80;
        return true;
    }
    return false;
}

// bool readPowerButtonStat(void)
bool powerbutton::readStat(void)
{
    if (flag & 0x40)
    {
        return true;
    }
    return false;
}