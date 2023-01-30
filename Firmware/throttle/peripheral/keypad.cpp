
#include "pico/stdlib.h"

#include "keypad.h"
#include "keypadCodeList.h"

#define KEY_ROW_SEL_1_PORT 8
#define KEY_ROW_SEL_2_PORT 9
#define KEY_ROW_SEL_3_PORT 10
#define KEY_ROW_SEL_4_PORT 11
#define KEY_COL_IN_1_PORT 12
#define KEY_COL_IN_2_PORT 13
#define KEY_COL_IN_3_PORT 14
#define KEY_COL_IN_4_PORT 15

#define KEY_ROW_PORT_MASK (1 << KEY_ROW_SEL_1_PORT) | (1 << KEY_ROW_SEL_2_PORT) | (1 << KEY_ROW_SEL_3_PORT) | (1 << KEY_ROW_SEL_4_PORT)
#define KEY_COL_PORT_MASK (1 << KEY_COL_IN_1_PORT) | (1 << KEY_COL_IN_2_PORT) | (1 << KEY_COL_IN_3_PORT) | (1 << KEY_COL_IN_4_PORT)

/*
uint8_t keypadReadCounter;
uint32_t keypadReadData;
uint8_t keypadScanCode;
uint8_t keypadScanFlag;
*/

uint8_t keypad::counter;
uint32_t keypad::readData;
uint8_t keypad::scanCode;
uint8_t keypad::scanFlag;

uint8_t keypadCodeScan(uint32_t scanData);

void keypad::init(void)
{
    gpio_init_mask(KEY_ROW_PORT_MASK | KEY_COL_PORT_MASK);
    gpio_set_dir_in_masked(KEY_COL_PORT_MASK);
    gpio_pull_up(KEY_COL_IN_1_PORT);
    gpio_pull_up(KEY_COL_IN_2_PORT);
    gpio_pull_up(KEY_COL_IN_3_PORT);
    gpio_pull_up(KEY_COL_IN_4_PORT);
    gpio_set_dir_out_masked(KEY_ROW_PORT_MASK);
    gpio_put_masked(KEY_ROW_PORT_MASK, 0);
    counter = 9;
}

void keypad::event(void)
{
    uint32_t gpioRead;
    uint8_t gpioStat = 0;
    uint8_t newKeyCode;

    counter++;
    if (counter >= 8)
    {
        counter = 0;
    }

    if (~counter & 0x01)
    {
        // Set Port
        //gpio_put_masked(KEY_ROW_PORT_MASK, 0);
        gpio_clr_mask(KEY_ROW_PORT_MASK);

        switch (counter & 0x06)
        {
        case 0x00:
            gpio_put(KEY_ROW_SEL_1_PORT, true);
            break;
        case 0x02:
            gpio_put(KEY_ROW_SEL_2_PORT, true);
            break;
        case 0x04:
            gpio_put(KEY_ROW_SEL_3_PORT, true);
            break;
        case 0x06:
            gpio_put(KEY_ROW_SEL_4_PORT, true);
            break;
        default:
            counter = 9;
            break;
        }
        /*
        if ((keypadReadCounter & 0x06) == 0x00)
        {
            gpio_put(KEY_ROW_SEL_1_PORT, true);
        }
        if ((keypadReadCounter & 0x06) == 0x02)
        {
            gpio_put(KEY_ROW_SEL_1_PORT, true);
        }
        if ((keypadReadCounter & 0x06) == 0x04)
        {
            gpio_put(KEY_ROW_SEL_1_PORT, true);
        }
        if ((keypadReadCounter & 0x06) == 0x06)
        {
            gpio_put(KEY_ROW_SEL_1_PORT, true);
        }
        */
    }
    else
    {
        // Read
        gpioRead = ~gpio_get_all();
        if (gpioRead & (1 << KEY_COL_IN_1_PORT))
            gpioStat |= 0x01;
        if (gpioRead & (1 << KEY_COL_IN_2_PORT))
            gpioStat |= 0x02;
        if (gpioRead & (1 << KEY_COL_IN_3_PORT))
            gpioStat |= 0x04;
        if (gpioRead & (1 << KEY_COL_IN_4_PORT))
            gpioStat |= 0x08;

        switch (counter & 0x06)
        {
        case 0x00:
            readData = gpioStat;
            break;
        case 0x02:
            readData |= (gpioStat << 4);
            break;
        case 0x04:
            readData |= (gpioStat << 8);
            break;
        case 0x06:
            readData |= (gpioStat << 12);
            newKeyCode = scan(readData);
            if (scanCode != newKeyCode)
            {
                scanCode = newKeyCode;
                scanFlag |= 0x01;
            }
            break;
        }
    }
}

uint8_t keypad::scan(uint32_t scanData)
{
    uint8_t i;

    for (i = 0; i < CODE_LIST_LENGTH; i++)
    {
        if (scanData == codeList[i].rawData)
        {
            return codeList[i].respCode;
        }
    }

    return 0xFF;
}

uint8_t keypad::checkChange(void)
{
    if (scanFlag & 0x01)
    {
        scanFlag &= ~0x01;
        return (1);
    }
    return (0);
}

uint8_t keypad::readCode(void)
{
    return (scanCode);
}