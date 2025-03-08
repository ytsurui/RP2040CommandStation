
#include "pico/stdlib.h"
#include "../../peripheral/i2c_ctrl.h"
#include <string.h>

#define LCD_RESET_PORT 1

void writeLCDcommand(char command)
{
    uint8_t i2cPacket[2];
    i2cPacket[0] = 0x00;
    i2cPacket[1] = command;
    writeI2Cpacket(0x3E, i2cPacket, 2);
}

void writeLCDdata(char data)
{
    uint8_t i2cPacket[2];
    i2cPacket[0] = 0x40;
    i2cPacket[1] = data;
    writeI2Cpacket(0x3E, i2cPacket, 2);
}

void writeLCDstring(char *str)
{
    uint8_t i;

    for (i = 0; i < strlen(str); i++)
    {
        writeLCDdata(str[i]);
    }
}

void showHex32Bit(uint32_t num)
{
    uint8_t i;
    uint8_t showValue;

    for (i = 0; i < 8; i++)
    {
        showValue = (num >> 28) & 0x0F;
        num <<= 4;
        if (showValue < 10)
        {
            writeLCDdata(0x30 + showValue);
        }
        else
        {
            writeLCDdata(0x37 + showValue);
        }
    }
}

void initLCD(void)
{
    gpio_init(LCD_RESET_PORT);
    gpio_set_dir(LCD_RESET_PORT, GPIO_OUT);

    gpio_put(LCD_RESET_PORT, true);
    sleep_ms(300);
    gpio_put(LCD_RESET_PORT, false);
    sleep_ms(300);
    gpio_put(LCD_RESET_PORT, true);
    sleep_ms(100);
    writeLCDcommand(0x38);
    writeLCDcommand(0x39);
    writeLCDcommand(0x14);
    writeLCDcommand(0x70);
    writeLCDcommand(0x56);
    writeLCDcommand(0x6C);
    writeLCDcommand(0x38);
    writeLCDcommand(0x0C);
    writeLCDcommand(0x01);
    sleep_ms(2);
}

void testMessage(void)
{
    writeLCDcommand(0x80);
    writeLCDdata(0x31);
    writeLCDdata(0x32);
    writeLCDdata(0x33);
    writeLCDdata(0x34);

    writeLCDcommand(0xC1);
    writeLCDdata(0x41);
    writeLCDdata(0x42);
    writeLCDdata(0x43);
    writeLCDdata(0x44);
}

bool _sendNum(uint16_t *value, uint16_t digit, bool printZero)
{
    uint8_t showValue;

    if (*value >= digit)
    {
        showValue = *value / digit;
        *value %= digit;

        writeLCDdata(0x30 + showValue);
        return true;
    }
    else if (printZero)
    {
        writeLCDdata(0x30);
        return true;
    }
    return false;
}

void sendNum(uint16_t value)
{
    uint8_t showValue;
    bool printZero = false;

    printZero = _sendNum(&value, 1000, printZero);
    printZero = _sendNum(&value, 100, printZero);
    printZero = _sendNum(&value, 10, printZero);
    writeLCDdata(0x30 + value);
}

void writeLCDsingleNum(uint8_t value)
{
    if (value <= 9)
    {
        writeLCDdata(0x30 + value);
    }
    else
    {
        sendSpaceLCD(1);
    }
}

void setLCDpos(uint8_t row, uint8_t column)
{
    if (row == 0)
    {
        writeLCDcommand(0x80 + column);
    }
    else if (row == 1)
    {
        writeLCDcommand(0xC0 + column);
    }
}

void sendSpaceLCD(uint8_t count)
{
    uint8_t i;

    for (i = 0; i < count; i++)
    {
        writeLCDdata(0x00);
    }
}

void clearLCD(void)
{
    // writeLCDcommand(0x01);
    setLCDpos(0, 0);
    sendSpaceLCD(16);
    setLCDpos(1, 0);
    sendSpaceLCD(16);
}

void showCursor(void)
{
    writeLCDcommand(0x0E);
}

void hideCursor(void)
{
    writeLCDcommand(0x0C);
}