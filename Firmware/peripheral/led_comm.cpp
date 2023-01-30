
#include "pico/stdlib.h"
#include "led_comm.h"

uint8_t commLED::counter;

void commLED::init(void)
{
    gpio_init(0);
    gpio_set_dir(0, GPIO_OUT);
    gpio_put(0, false);
    counter = 0;
}

void commLED::event(void)
{
    if (counter > 0)
    {
        counter++;

        if (counter >= COMMLED_MAX_COUNT)
        {
            gpio_put(0, false);
            counter = 0;
        }
    }
}

void commLED::set(void)
{
    if (counter == 0)
    {
        counter = 1;
        gpio_put(0, true);
    }
}