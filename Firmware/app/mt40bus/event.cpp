#include "pico/stdlib.h"
#include "mt40bus.h"

uint32_t mt40busCtrl::lastSendRecvCount;

void mt40busCtrl::eventMS(void)
{
    lastSendRecvCount++;

    if (lastSendRecvCount > LAST_SEND_RECV_COUNT_TIMEOUT) {
        lastSendRecvCount = 0;
        sendCmdEcho();
    }
}