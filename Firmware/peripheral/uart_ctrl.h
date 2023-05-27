
class uartCtrl
{
public:
    static void globalInit(void);
    static void globalTask(void);

    static uartCtrl *getInstance(uint8_t id);

    void init(void);
    void task(void);
    void event(void);

    void send(uint8_t data);

    void setBaudRate(int baud);
    void setRecvCallback(void (*cb)(uint8_t));

    static void bridgeMode(bool stat);


    static void BusGPIOdir(uint8_t num, bool dir, bool pullup);
    static void putBusGPIO(uint8_t num, bool value);
    static bool getBusGPIO(uint8_t num);

private:
    static uartCtrl uartCtrlInst[2];
    static bool bridgeModeStat;

    uart_inst_t *uartModule;
    uint baudRate;
    uint gpio_tx_num;
    uint gpio_rx_num;

    void (*recvCb)(uint8_t);
};