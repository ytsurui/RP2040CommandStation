
#include "pico/stdlib.h"
#include "eventTimer.h"

uint8_t eventtimer::eventFlag;
bool eventtimer::eventFlagMutex = false;
alarm_pool_t *eventtimer::eventAlarmPool;

#define EVENT_FLAG_MS 0x01
#define EVENT_FLAG_US 0x02

repeating_timer_t eventtimer::eventTimer;
/*
bool eventtimer::intMsEventTimer(repeating_timer_t *rt)
{
    eventFlag |= EVENT_FLAG_MS;
    return true;
}

void eventtimer::init(void)
{
    add_repeating_timer_ms(-1, intMsEventTimer, NULL, &eventTimer);
}
*/

void eventtimer::init(void)
{
    //add_alarm_in_ms(1, msEventTimerHW, NULL, false);
    eventAlarmPool = alarm_pool_create(1, 8);
    alarm_pool_add_repeating_timer_ms(eventAlarmPool, -1, interruptMsEventTimer, NULL, &eventTimer);
}

int64_t eventtimer::msEventTimerHW(alarm_id_t id, void *user_data)
{
    eventFlagMutex = true;
    eventFlag |= EVENT_FLAG_MS;
    eventFlagMutex = false;
    return 1000;
}

bool eventtimer::interruptMsEventTimer(repeating_timer_t *rt)
{
    eventFlag |= EVENT_FLAG_MS;
    return true;
}

bool eventtimer::checkMS(void)
{
    while (eventFlagMutex)
        ;

    eventFlagMutex = true;
    if (eventFlag & EVENT_FLAG_MS)
    {
        eventFlag &= ~EVENT_FLAG_MS;
        eventFlagMutex = false;
        return true;
    }
    eventFlagMutex = false;
    return false;
}

bool eventtimer::checkNS(void)
{
    while (eventFlagMutex)
        ;

    eventFlagMutex = true;
    if (eventFlag & EVENT_FLAG_US)
    {
        eventFlag &= ~EVENT_FLAG_US;
        eventFlagMutex = false;
        return true;
    }
    eventFlagMutex = false;
    return false;
}
