
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define I2C_SDA_PORT 2
#define I2C_SCL_PORT 3

void initI2Cport(void)
{
    i2c_init(i2c1, 100000);
    gpio_set_function(I2C_SDA_PORT, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PORT, GPIO_FUNC_I2C);
    gpio_set_pulls(I2C_SDA_PORT, false, false);
    gpio_set_pulls(I2C_SCL_PORT, false, false);
}

uint8_t writeI2Cpacket(uint8_t addr, uint8_t *packet, uint8_t length)
{
    return i2c_write_timeout_us(i2c1, addr, packet, length, false, 2000);
}