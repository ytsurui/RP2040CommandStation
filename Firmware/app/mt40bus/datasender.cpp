
#include "pico/stdlib.h"
#include "mt40bus.h"
#include "../../peripheral/dccsignalport.h"

#include <stdio.h>

mt40busCtrl::cb_info mt40busCtrl::senderCb;
mt40busCtrl::cb_bool_info mt40busCtrl::carrierSenseCb;

void mt40busCtrl::setSender(void (*method)(uint8_t))
{
    senderCb.func = method;
    senderCb.assigned = true;
}

void mt40busCtrl::setCarrierSenseFunc(bool (*method)(void))
{
    carrierSenseCb.func = method;
    carrierSenseCb.assigned = true;
}

void mt40busCtrl::sendCmd(uint32_t cmd, uint32_t *args, uint8_t length)
{
    sendCmd(cmd, args, length, false);
}

void mt40busCtrl::sendCmd(uint32_t cmd, uint32_t *args, uint8_t length, bool privatePacket)
{
    uint8_t byteData, i, i2, datLength;
    uint8_t sendData[64];
    uint8_t ASCIIdata[10];
    uint8_t byteLength = 0;
    bool separateFlag;

    if (cmd == 0) return;

    if (carrierSenseCb.assigned) {
        //printf("carrier-sense...\n");
        sleep_ms(5);
        //while (!carrierSenseCb.func()) {
            //sleep_us(10);
            //printf("cb-wait\n");
        //    sleep_ms(7);
        //}
    }

    sendData[byteLength] = '\r';
    byteLength++;
    sendData[byteLength] = '\n';
    byteLength++;

    //printf("data send cmd: %d\n", cmd);

    while (cmd != 0) {
        byteData = (cmd & 0xFF000000) >> 24;
        if (byteData != 0) {
            sendData[byteLength] = byteData;
            byteLength++;
        }

        cmd = cmd << 8;
        if (cmd == 0) break;
    }

    if (privatePacket) {
        sendData[byteLength] = '{';
    } else {
        sendData[byteLength] = '(';
    }
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

    if (privatePacket) {
        sendData[byteLength] = '}';
    } else {
        sendData[byteLength] = ')';
    }
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

    //printf("send complete\n");
}

void mt40busCtrl::sendCmdPWSresp(void)
{
    uint32_t sendArg[1];

    if (dccport::getPowerStat()) {
        sendArg[0] = 1;
    } else {
        sendArg[0] = 0;
    }

    sendCmd('PWS', sendArg, 1);
}

void mt40busCtrl::sendCmdEcho(void)
{
    sendCmd('ECHO', nullptr, 0);
}

void mt40busCtrl::sendCmdPMUP(uint32_t addr)
{
    if (addr == 0) {
        sendCmd('PMUP', nullptr, 0);
        return;
    }

    uint32_t sendArg[1];

    sendArg[0] = addr;

    sendCmd('PMUP', sendArg, 1);
}

void mt40busCtrl::sendCmdPMSTresp(uint32_t addr, bool stat)
{
    if (addr == 0) {
        return;
    }

    uint32_t sendArg[2];
    sendArg[0] = addr;
    if (stat) {
        sendArg[1] = 1;
    } else {
        sendArg[0] = 0;
    }
    sendCmd('PMST', sendArg, 2);
}

void mt40busCtrl::encodeNumToASCIIoct(uint32_t src, uint8_t *dest, uint8_t *length)
{
    uint8_t pos = 0;

    if (src >= 1000000000) {
        dest[pos] = '0' + (uint8_t)(src / 1000000000);
        src = src % 1000000000;
        pos++;
    }

    if (src >= 100000000) {
        dest[pos] = '0' + (uint8_t)(src / 100000000);
        src = src % 100000000;
        pos++;
    }

    if (src >= 10000000) {
        dest[pos] = '0' + (uint8_t)(src / 10000000);
        src = src % 10000000;
        pos++;
    }

    if (src >= 1000000) {
        dest[pos] = '0' + (uint8_t)(src / 1000000);
        src = src % 1000000;
        pos++;
    }

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

char mt40busCtrl::encodeNumToHexStr(uint8_t data)
{
    if (data <= 0x09) {
        return ('0' + data);
    }
    if (data <= 0x0F) {
        return ('A' + (data - 10));
    }

    return 0;
}