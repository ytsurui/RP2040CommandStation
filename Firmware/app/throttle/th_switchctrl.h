
class throttleAppSwitchCtrl
{
public:
    static void init(void);
    static void event(void);
    static void task(void);

    static void dispInit(void);
    static void dispEvent(void);
    static void dispTerminate(void);

    static void keypadEvent(uint8_t code);

private:
    static uint16_t selectSWaddr;
    static bool inputFlag;
};