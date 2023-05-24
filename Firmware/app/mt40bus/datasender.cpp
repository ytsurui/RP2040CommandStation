
#include "pico/stdlib.h"
#include "mt40bus.h"

mt40busCtrl::cb_info mt40busCtrl::senderCb;

void mt40busCtrl::setSender(void (*method)(uint8_t))
{
    senderCb.func = method;
    senderCb.assigned = true;
}

void mt40busCtrl::sendCmd(uint32_t cmd, uint32_t *args, uint8_t length)
{
    uint8_t byteData, i, i2, datLength;
    uint8_t sendData[64];
    uint8_t ASCIIdata[6];
    uint8_t byteLength = 0;
    bool separateFlag;

    if (cmd == 0) return;

    while (cmd != 0) {
        byteData = (cmd & 0xFF000000) >> 24;
        if (byteData != 0) {
            sendData[byteLength] = byteData;
            byteLength++;
        }

        cmd = cmd << 8;
        if (cmd == 0) break;
    }

    sendData[byteLength] = '(';
    byteLength++;
    separateFlag = false;

    for (i = 0; i < length; i++) {
        if (separateFlag) {
            sendData[byteLength] = ',';
            byteLength++;
        }

        encodeNumToASCIIoct(args[i], ASCIIdata, &datLength);

        for (i2 = 0; i2 < datLength; i2++) {
            sendData[byteLength] = ASCIIdata[i2];
            byteLength++;
        }
        separateFlag = true;
    }

    sendData[byteLength] = ')';
    byteLength++;

    sendData[byteLength] = '\r';
    byteLength++;
    sendData[byteLength] = '\n';
    byteLength++;

    if (senderCb.assigned) {
        for (i = 0; i < byteLength; i++) {
            senderCb.func(sendData[i]);
            //printf("index: %d, data: %c\n", i, sendData[i]);
        }
    //} else {
    //    printf("sender callback not assigned\n");
    }

    //printf("send complete\n");
}

void mt40busCtrl::encodeNumToASCIIoct(uint32_t src, uint8_t *dest, uint8_t *length)
{
    uint8_t pos = 0;

    if (src >= 100000) {
        dest[pos] = '0' + (uint8_t)(src / 100000);
        src = src % 100000;
        pos++;
    }
 
    if ((src >= 10000) || (pos > 0)) {
        dest[pos] = '0' + (uint8_t)(src / 10000);
        src = src % 10000;
        pos++;
    }

    if ((src >= 1000) || (pos > 0)) {
        dest[pos] = '0' + (uint8_t)(src / 1000);
        src = src % 1000;
        pos++;
    }

    if ((src >= 100) || (pos > 0)) {
        dest[pos] = '0' + (uint8_t)(src / 100);
        src = src % 100;
        pos++;
    }

    if ((src >= 10) || (pos > 0)) {
        dest[pos] = '0' + (uint8_t)(src / 10);
        src = src % 10;
        pos++;
    }

    dest[pos] = '0' + (uint8_t)src;
    pos++;

    *length = pos;
}
