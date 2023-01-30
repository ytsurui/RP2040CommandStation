
#define CURR_VALUE_ARRAY_LENGTH 8

class currentMonitor
{
public:
    static void init(void);
    static void event(void);
    static void task(void);
    static void print(void);
    static uint16_t getCurrent(void);

private:
    static uint8_t counter;
    static uint16_t currentValue;
    static uint16_t overCurrentCounter;
    static uint16_t overCurrentReturnCheckCounter;
    static uint16_t currentAdcOffsetValue;

    static uint16_t viewCurrentValue;
    static uint16_t currValueArray[CURR_VALUE_ARRAY_LENGTH];
    static uint8_t currValueIndex;
};
