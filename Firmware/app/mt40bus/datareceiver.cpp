
#include "pico/stdlib.h"
#include "mt40bus.h"

#include <stdio.h>
#include <string.h>

mt40busCtrl::packetBuf mt40busCtrl::recvData;
mt40busCtrl::packetBuf mt40busCtrl::execData;

void mt40busCtrl::init()
{
    //recvData.bufpos = 0;
    recvData.length = 0;
}

void mt40busCtrl::recv(uint8_t rData)
{
    if (rData == '\r') {
        // Ignore
        return;
    }
    if (rData == '\n') {
        // Data End
        bufCopy(&recvData, &execData);
        recvData.length = 0;

        if (execData.length == 0) {
            return;
        }

        // Execution
        if (execData.length < 4) {
            return;
        }
        
        // Parse
        execPacket();
        return;
    }

    if (recvData.length >= MT40BUS_BUF_LENGTH) {
        return;
    }

    recvData.Buf[recvData.length] = rData;
    recvData.length++;
}

void mt40busCtrl::bufCopy(packetBuf *src, packetBuf *dest)
{
    uint8_t i;

    for (i = 0; i < src->length; i++) {
        dest->Buf[i] = src->Buf[i];
    }
    dest->length = src->length;
}

#define EXEC_PACKET_MODE_CMD    1
#define EXEC_PACKET_MODE_ARGS   2

void mt40busCtrl::execPacket()
{
    uint8_t i;
    bool cmdChkFlag;
    bool cmdExitFlag;
    uint8_t mode;
    uint8_t decodeData;
    bool enableHEX;
    uint8_t argGetCount;
    uint8_t argIndex = 0;

    uint32_t cmdData;
    uint32_t argTable[10];

    // Parse
    //if (execData.Buf[0] != '(' && execData.Buf[0] != '{') {
    //    return;
    //}

    //if (execData.Buf[execData.length - 1] != ')' && execData.Buf[execData.length - 1] != '}') {
    //    return;
    //}



    //packetHead[0] = execData.Buf[0];
    //packetHead[1] = execData.Buf[1];

    cmdChkFlag = false;
    cmdExitFlag = false;
    cmdData = 0;
    argGetCount = 0;
    mode = EXEC_PACKET_MODE_CMD;

    for (i = 0; i < execData.length; i++) {
        switch (mode) {
            case EXEC_PACKET_MODE_CMD:    
                if (i > 3) return;
                if (execData.Buf[i] == '(' || execData.Buf[i] == '{') {
                    cmdChkFlag = true;
                    mode = EXEC_PACKET_MODE_ARGS;
                    break;
                }
                cmdData = (cmdData << 8) || execData.Buf[i];
                break;
            case EXEC_PACKET_MODE_ARGS:
                switch (execData.Buf[i]) {
                    case ',':
                        // ARG END
                        enableHEX = false;
                        argGetCount = 0;
                        argIndex++;
                        break;
                    case '}':
                    case ')':
                        // Command END
                        cmdExitFlag = true;
                        argIndex++;
                        break;
                    case 'x':
                    case 'X':
                        if (argGetCount == 1 && argTable[argIndex] == 0) {
                            enableHEX = true;
                            argGetCount = 0;
                        }
                        break;
                    default:
                        decodeData = decodeASCIItoNum(execData.Buf[i], enableHEX);
                        if (decodeData == 0xFF)
                            return;
                        if (enableHEX) {
                            // 16進数表記
                            argTable[argIndex] = (argTable[argIndex] << 4) | decodeData;
                        } else {
                            // 10進数表記
                            argTable[argIndex] = (argTable[argIndex] * 10) + decodeData;
                        }
                        argGetCount++;
                        break;
                }
                break;
            
            if (cmdExitFlag) {
                // コマンド終了フラグを確認
                break;
            }
        }
    }
    if (!cmdChkFlag || !cmdExitFlag) return;


    switch (cmdData) {
        case 'PW':
            // Power
            execCmdPW(argTable, argIndex);            
            break;
        case 'PWT':
            // Power Toggle
            execCmdPWT(argTable, argIndex);
            break;
        case 'PWS':
            // Power Status
            execCmdPWS(argTable, argIndex);
            break;
        case 'DI':
            // Train Direction
            execCmdDI(argTable, argIndex);
            break;
        case 'FN':
            // Train Function
            execCmdFN(argTable, argIndex);
            break;
        case 'SP':
            // Train Speed
            execCmdSP(argTable, argIndex);
            break;
        case 'TO':
            // Turnout (Accessory Decoders)
            execCmdTO(argTable, argIndex);
            break;
        case 'TOS':
            // Turnout (Accessory Decoders) Status
            execCmdTOS(argTable, argIndex);
            break;
        
    }

}

