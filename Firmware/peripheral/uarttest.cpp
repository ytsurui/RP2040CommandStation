
#include "pico/stdlib.h"
#include "uarttest.h"

#include "led_comm.h"

uartTest::cb_info uartTest::cbdata;

void uartTest::init(void)
{
    gpio_init(16);
    gpio_init(17);
    gpio_pull_up(16);
    gpio_pull_up(17);
    gpio_set_dir(16, false);
    gpio_set_dir(17, false);

    // Wireless: uart0 / bus: uart1
    uart_init(uart0, 19200);
    //gpio_pull_up(16);
    //gpio_pull_up(17);
    gpio_set_function(16, GPIO_FUNC_UART);
    gpio_set_function(17, GPIO_FUNC_UART);
    uart_is_enabled(uart0);

    //gpio_put(0, true);

    cbdata.assigned = false;
}

void uartTest::task(void)
{
    uint8_t recv;

    while (uart_is_readable(uart0))
    {
        commLED::set();

        recv = uart_getc(uart0);
        if (cbdata.assigned)
        {
            cbdata.func(recv);
        }
    }
}

void uartTest::setCallback(void (*cb)(uint8_t))
{
    cbdata.func = cb;
    cbdata.assigned = true;
}

void uartTest::send(uint8_t data)
{
    while (!uart_is_writable(uart0))
        ;
    uart_putc(uart0, data);
}