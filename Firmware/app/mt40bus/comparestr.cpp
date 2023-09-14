
#include "pico/stdlib.h"
#include "mt40bus.h"

bool mt40busCtrl::compareStr(char *arg1, uint8_t length1, char *arg2, uint8_t length2)
{
    uint8_t i;

    if (length1 != length2) {
        return false;
    }

    for (i = 0; i < length1; i++) {
        if (arg1[i] != arg2[i]) return false;
    }

    return true;
}

uint8_t mt40busCtrl::decodeASCIItoNum(char data, bool enHEX)
{
    switch (data) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return (data - '0');
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
            if (enHEX) {
                return (0x0A + data - 'a');
            }
            break;
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
            if (enHEX) {
                return (0x0A + data - 'A');
            }
            break;
    }

    return (0xFF);
}