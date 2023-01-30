
class throttleApp
{
public:
    static void event(void);

private:
    static uint8_t throttle1Value;
    static uint8_t throttle2Value;

    static uint8_t throttle1LastSend;
    static uint8_t throttle2LastSend;
};