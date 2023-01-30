
class voltageMonitor
{
public:
    static void init(void);
    static void event(void);
    static uint16_t get(void);

private:
    static uint16_t counter;
    static uint16_t mv;

    static uint16_t mvInit;
};