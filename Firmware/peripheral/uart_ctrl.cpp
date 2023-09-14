
#include "pico/stdlib.h"
#include "uart_ctrl.h"

#define BUS_TX_GPIO 4
#define BUS_RX_GPIO 5
#define BUS_CTRL1_GPIO 6
#define BUS_CTRL2_GPIO 7
#define WIRELESS_TX_GPIO 16
#define WIRELESS_RX_GPIO 17

#define DEFAULT_BAUDRATE 115200

bool uartCtrl::bridgeModeStat;
uartCtrl uartCtrl::uartCtrlInst[2];

void uartCtrl::globalInit(void)
{
    // Wireless Port Pre-Init (for XBee3)
    gpio_init(WIRELESS_TX_GPIO);
    gpio_init(WIRELESS_RX_GPIO);
    gpio_pull_up(WIRELESS_TX_GPIO);
    gpio_pull_up(WIRELESS_RX_GPIO);
    gpio_set_dir(WIRELESS_TX_GPIO, false);
    gpio_set_dir(WIRELESS_RX_GPIO, false);

    gpio_init(BUS_TX_GPIO);
    gpio_init(BUS_RX_GPIO);
    gpio_pull_up(BUS_TX_GPIO);
    gpio_pull_up(BUS_RX_GPIO);
    gpio_set_dir(BUS_TX_GPIO, false);
    gpio_set_dir(BUS_RX_GPIO, false);

    uartCtrlInst[0].uartModule = uart1;
    uartCtrlInst[1].uartModule = uart0;

    uartCtrlInst[0].gpio_rx_num = BUS_RX_GPIO;
    uartCtrlInst[0].gpio_tx_num = BUS_TX_GPIO;
    uartCtrlInst[1].gpio_rx_num = WIRELESS_RX_GPIO;
    uartCtrlInst[1].gpio_tx_num = WIRELESS_TX_GPIO;

    uartCtrlInst[0].init();
    uartCtrlInst[1].init();
}

void uartCtrl::globalTask(void)
{
    uartCtrlInst[0].task();
    uartCtrlInst[1].task();
}

uartCtrl *uartCtrl::getInstance(uint8_t id)
{
    if (id >= 2)
    {
        return nullptr;
    }
    return &uartCtrlInst[id];
}

void uartCtrl::init(void)
{
    if (baudRate == 0)
    {
        baudRate = DEFAULT_BAUDRATE;
    }
    recvCb = nullptr;
    gpio_init(gpio_tx_num);
    gpio_init(gpio_rx_num);
    uart_init(uartModule, baudRate);
    gpio_set_function(gpio_tx_num, GPIO_FUNC_UART);
    gpio_set_function(gpio_rx_num, GPIO_FUNC_UART);
    uart_is_enabled(uartModule);
}

void uartCtrl::task(void)
{
    uint8_t recv;

    while (uart_is_readable(uartModule))
    {
        recv = uart_getc(uartModule);
        if (recvCb != nullptr)
        {
            recvCb(recv);
        }
    }
}

void uartCtrl::send(uint8_t data)
{
    while (!uart_is_writable(uartModule))
        ;
    uart_putc(uartModule, data);
}

void uartCtrl::setBaudRate(int baud)
{
    baudRate = baud;
    uart_init(uartModule, baudRate);
    uart_is_enabled(uartModule);
}

void uartCtrl::setRecvCallback(void (*cb)(uint8_t))
{
    recvCb = cb;
}

void uartCtrl::BusGPIOdir(uint8_t num, bool dir, bool pullup)
{
    uint8_t gpioNum;
    if (num == 0) {
        gpioNum = BUS_CTRL1_GPIO;
    } else if (num == 1) {
        gpioNum = BUS_CTRL2_GPIO;
    } else {
        return;
    }

    gpio_set_dir(gpioNum, dir);
    if (!dir && pullup) {
        gpio_pull_up(gpioNum);
    } 
}

void uartCtrl::putBusGPIO(uint8_t num, bool value)
{
    uint8_t gpioNum;
    if (num == 0) {
        gpioNum = BUS_CTRL1_GPIO;
    } else if (num == 1) {
        gpioNum = BUS_CTRL2_GPIO;
    } else {
        return;
    }

    gpio_put(gpioNum, value);
}

bool uartCtrl::getBusGPIO(uint8_t num)
{
    uint8_t gpioNum;
    if (num == 0) {
        gpioNum = BUS_CTRL1_GPIO;
    } else if (num == 1) {
        gpioNum = BUS_CTRL2_GPIO;
    } else {
        return false;
    }

    return gpio_get(gpioNum);
}