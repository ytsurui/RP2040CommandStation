

#define COMMLED_MAX_COUNT 10

class commLED
{
public:
    static void init(void);
    static void event(void);
    static void set(void);

private:
    static uint8_t counter;
};