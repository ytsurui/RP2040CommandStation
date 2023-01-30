
class powerbutton
{
public:
    static void init(void);
    static void event(void);
    static bool checkChange(void);
    static bool readStat(void);

private:
    static uint8_t counter;
};
/*
void initPowerButton(void);
void eventPowerButton(void);
bool checkChangePowerButtonStat(void);
bool readPowerButtonStat(void);
*/