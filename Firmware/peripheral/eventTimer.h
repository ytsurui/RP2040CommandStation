
class eventtimer
{
public:
    static void init(void);
    static bool checkMS(void);
    static bool checkNS(void);

private:
    static bool intMsEventTimer(repeating_timer_t *rt);
    static bool interruptMsEventTimer(repeating_timer_t *rt);

    static int64_t msEventTimerHW(alarm_id_t id, void *user_data);

    static uint8_t eventFlag;
    static bool eventFlagMutex;
    static repeating_timer_t eventTimer;

    static alarm_pool_t *eventAlarmPool;
};

/*
bool intMsEventTimer(repeating_timer_t *rt);
void initEventTimer(void);
bool chkEventTimerMS(void);
bool chkEventTimerNS(void);
*/