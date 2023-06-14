
#include "pico/stdlib.h"
#include "mt40bus.h"

#ifdef ENABLE_LBUS

mt40busCtrl::cb_info mt40busCtrl::LBUSrecvCb;
mt40busCtrl::cb_info mt40busCtrl::LBUSsendCb;

void mt40busCtrl::setLBUScommandReceiver(void (*func)(uint8_t))
{
    LBUSrecvCb.func = func;
    LBUSrecvCb.assigned = true;
}

void mt40busCtrl::setLBUScommandSender(void (*func)(uint8_t))
{
    LBUSsendCb.func = func;
    LBUSsendCb.assigned = true;
}

void mt40busCtrl::sendLBUSdata(uint8_t *packet, uint8_t length)
{
    uint8_t byteData, i, i2, datLength;
    uint8_t sendData[64];
    uint8_t data;
    uint8_t byteLength;

    if (carrierSenseCb.assigned) {
        //printf("carrier-sense...\n");
        sleep_ms(5);
        while (carrierSenseCb.func()) {
            //printf("cb-wait\n");
        }
    }

    sendData[0] = 'L';
    sendData[1] = 'B';
    sendData[2] = 'U';
    sendData[3] = 'S';

#ifdef SET_LBUS_LOCAL_COMMAND
    sendData[4] = '{';
#else
    sendData[4] = '(';
#endif
    byteLength = 5;

    for (i = 0; i < length; i++) {
        data = (packet[i] & 0xF0) >> 4;
        sendData[byteLength] = encodeNumToHexStr(data);
        byteLength++;
        
        data = packet[i] & 0x0F;
        sendData[byteLength] = encodeNumToHexStr(data);
        byteLength++;
    }

#ifdef SET_LBUS_LOCAL_COMMAND
    sendData[byteLength] = '}';
#else
    sendData[byteLength] = ')';
#endif
    byteLength++;

    sendData[byteLength] = '\r';
    byteLength++;
    sendData[byteLength] = '\n';
    byteLength++;

     if (senderCb.assigned) {
        for (i = 0; i < byteLength; i++) {
            senderCb.func(sendData[i]);
            lastSendRecvCount = 0;
            //printf("index: %d, data: %c\n", i, sendData[i]);
        }
    //} else {
    //    printf("sender callback not assigned\n");
    }
}
#endif