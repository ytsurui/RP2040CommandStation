/*
#ifdef __cplusplus
extern "C" void initKeypadCtrlPort(void);
extern "C" void eventKeypadCtrl(void);

extern "C" uint8_t keypadChangeInput(void);
extern "C" uint8_t keypadReadScanCode(void);
#else
void initKeypadCtrlPort(void);
void eventKeypadCtrl(void);

uint8_t keypadChangeInput(void);
uint8_t keypadReadScanCode(void);
#endif
*/

class keypad
{
public:
    static void init(void);
    static void event(void);

    static uint8_t checkChange(void);
    static uint8_t readCode(void);

private:
    static uint8_t counter;
    static uint8_t counter2;
    static uint32_t readData;
    static uint8_t scanCode;
    static uint8_t scanFlag;
    static uint8_t scan(uint32_t scanData);
};

#define KEYCODE_NUM_0 0
#define KEYCODE_NUM_1 1
#define KEYCODE_NUM_2 2
#define KEYCODE_NUM_3 3
#define KEYCODE_NUM_4 4
#define KEYCODE_NUM_5 5
#define KEYCODE_NUM_6 6
#define KEYCODE_NUM_7 7
#define KEYCODE_NUM_8 8
#define KEYCODE_NUM_9 9

#define KEYCODE_FUNC_A 11
#define KEYCODE_FUNC_B 12
#define KEYCODE_FUNC_C 13
#define KEYCODE_FUNC_D 14

#define KEYCODE_FUNC_ASTERISK 21
#define KEYCODE_FUNC_SHARP 22
#define KEYCODE_FUNC_SWITCH 27
#define KEYCODE_FUNC_PROG 28

#define KEYCODE_WITH_ASTERISK 50
#define KEYCODE_WITH_SHARP 60