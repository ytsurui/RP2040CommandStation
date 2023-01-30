
#include "pico/stdlib.h"
#include "fail_led.h"

//uint16_t failureLEDcounter = 0;
//bool enableFailureLED = false;
uint16_t failureLED::counter = 0;
bool failureLED::enable = false;

//void initFailureLED(void)
void failureLED::init(void)
{
    gpio_init(24);
    gpio_set_dir(24, GPIO_OUT);
}

//void eventFailureLED(void)
void failureLED::event(void)
{
    if (enable)
    {
        // Enable
        counter++;
        if (counter >= 1500)
        {
            counter = 0;
        }
        switch (counter)
        {
        case 100:
        case 300:
            gpio_put(24, true);
            break;
        case 200:
        case 400:
            gpio_put(24, false);
            break;
        }
    }
    else
    {
        counter = 0;
        gpio_put(24, false);
    }
}

//void setFailureLEDstat(bool newStat)
void failureLED::setStat(bool newStat)
{
    enable = newStat;
}