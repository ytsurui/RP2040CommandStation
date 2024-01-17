#include "pico/stdlib.h"
#include "mt40bus.h"

uint32_t mt40busCtrl::lastSendRecvCount;
uint32_t mt40busCtrl::sendPMUPcounter = 0;

void mt40busCtrl::eventMS(void)
{
    lastSendRecvCount++;

    if (lastSendRecvCount > LAST_SEND_RECV_COUNT_TIMEOUT) {
        lastSendRecvCount = 0;
        sendCmdEcho();
    }

    if (sendPMUPcounter > 0) {
        sendPMUPcounter++;

        if (sendPMUPcounter >= SEND_PMUP_COUNTER_MAX) {
            sendPMUPcounter = 0;
            sendCmdPMUP(0);
            sendCmdPMUP(0);
            sendCmdPMUP(0);
            sendCmdPMUP(0);
            clearPMdownStat(0);
        }
    }
}