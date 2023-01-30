
class failureLED
{
public:
    static void init(void);
    static void event(void);
    static void setStat(bool newStat);

private:
    static uint16_t counter;
    static bool enable;
};

/*
void initFailureLED(void);
void eventFailureLED(void);
void setFailureLEDstat(bool newStat);
*/