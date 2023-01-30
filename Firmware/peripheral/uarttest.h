
class uartTest
{
public:
    static void init(void);
    static void task(void);

    static void setCallback(void (*cb)(uint8_t));

    static void send(uint8_t data);

private:
    typedef struct
    {
        void (*func)(uint8_t);
        bool assigned;
    } cb_info;

    static cb_info cbdata;
};