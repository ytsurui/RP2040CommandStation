

#include "pico/stdlib.h"
#include "dccsignalport.h"
#include "fail_led.h"

#define DCC_SIGNAL_PORT_1 18
#define DCC_SIGNAL_PORT_2 19

uint8_t dccport::powerStat = DCC_POWER_STAT_OFF;
bool dccport::execCutout = false;

bool dccport::progMode = false;

uint16_t dccport::powerInCount;

void dccport::init(void)
{
    gpio_init(DCC_SIGNAL_PORT_1);
    gpio_init(DCC_SIGNAL_PORT_2);

    gpio_set_dir(DCC_SIGNAL_PORT_1, GPIO_OUT);
    gpio_set_dir(DCC_SIGNAL_PORT_2, GPIO_OUT);

    powerStat = DCC_POWER_STAT_OFF;
}

void dccport::event(void)
{
    if (powerInCount > 0)
    {
        powerInCount++;
        if (powerInCount > DCC_POWER_IN_INIT_COUNT)
        {
            powerInCount = 0;
        }
    }
}

void dccport::_dcc_send_pulse(uint16_t waitTimeR, uint16_t waitTimeL)
{
    if (powerStat != DCC_POWER_STAT_ON)
    {
        gpio_put(DCC_SIGNAL_PORT_1, false);
        gpio_put(DCC_SIGNAL_PORT_2, false);
        return;
    }
    gpio_put(DCC_SIGNAL_PORT_1, false);
    gpio_put(DCC_SIGNAL_PORT_2, true);
    sleep_us(waitTimeR);
    gpio_put(DCC_SIGNAL_PORT_1, true);
    gpio_put(DCC_SIGNAL_PORT_2, false);
    sleep_us(waitTimeL);
}

void dccport::dcc_send_pulse(uint8_t value)
{
    if (value)
    {
        _dcc_send_pulse(58, 58);
    }
    else
    {
        _dcc_send_pulse(100, 100);
    }
}

void dccport::dcc_railcom_cutout(void)
{
    if (powerStat != DCC_POWER_STAT_ON)
    {
        gpio_put(DCC_SIGNAL_PORT_1, false);
        gpio_put(DCC_SIGNAL_PORT_2, false);
        return;
    }
    execCutout = true;
    gpio_put(DCC_SIGNAL_PORT_1, false);
    gpio_put(DCC_SIGNAL_PORT_2, true);
    sleep_us(28);

    if (powerStat != DCC_POWER_STAT_ON)
    {
        gpio_put(DCC_SIGNAL_PORT_1, false);
        gpio_put(DCC_SIGNAL_PORT_2, false);

        execCutout = false;
        return;
    }
    gpio_put(DCC_SIGNAL_PORT_1, true);
    sleep_us(470);

    if (powerStat != DCC_POWER_STAT_ON)
    {
        gpio_put(DCC_SIGNAL_PORT_1, false);
        gpio_put(DCC_SIGNAL_PORT_2, false);

        execCutout = false;
        return;
    }

    execCutout = false;
}

void dccport::dcc_send_preamble(void)
{
    uint8_t i;

    if (progMode)
    {
        for (i = 0; i < 7; i++)
        {
            dcc_send_pulse(1);
        }
    }

    for (i = 0; i < 17; i++)
    // for (i = 0; i < 12; i++)
    {
        dcc_send_pulse(1);
    }
}

void dccport::dcc_send_packet(uint8_t *array, uint8_t length, uint8_t cycle)
{
    uint8_t cy, i, i2, sendByte;

    for (cy = 0; cy < cycle; cy++)
    {
        dcc_send_preamble();
        dcc_send_pulse(0);

        for (i = 0; i < length; i++)
        {
            sendByte = array[i];

            for (i2 = 0; i2 < 8; i2++)
            {
                dcc_send_pulse(sendByte & 0x80);
                sendByte = sendByte << 1;
            }

            if (i < (length - 1))
            {
                dcc_send_pulse(0);
            }
            else
            {
                dcc_send_pulse(1);
            }
        }
    }
}

void dccport::setPowerStat(bool stat)
{
    if (stat)
    {
        powerStat = DCC_POWER_STAT_ON;
        powerInCount = 1;
        failureLED::setStat(false);
    }
    else
    {
        powerStat = DCC_POWER_STAT_OFF;
        execCutout = false;
        gpio_put(DCC_SIGNAL_PORT_1, false);
        gpio_put(DCC_SIGNAL_PORT_2, false);
    }
}

void dccport::togglePowerStat(void)
{
    if (powerStat == DCC_POWER_STAT_OFF)
    {
        powerStat = DCC_POWER_STAT_ON;
        powerInCount = 1;
        failureLED::setStat(false);
    }
    else
    {
        powerStat = DCC_POWER_STAT_OFF;
        execCutout = false;
        gpio_put(DCC_SIGNAL_PORT_1, false);
        gpio_put(DCC_SIGNAL_PORT_2, false);
    }
}

bool dccport::dcc_railcom_cutout_running(void)
{
    return execCutout;
}

// if true, Service-Mode (Long-Preamble)
void dccport::setProgmode(bool stat)
{
    progMode = stat;
}

bool dccport::powerInitCheck(void)
{
    return (powerInCount > 0);
}