#include "pico/stdlib.h"
#include "mt40bus.h"
#include "../dccsignal.h"

void mt40busCtrl::execCmdCO(uint32_t *args, uint8_t argCount)
{
    if (argCount == 0) {
        uint32_t sendArg[1];

        if (dccsignal::GetBiDiCutoutStat()) {
            sendArg[0] = 1;
        } else {
            sendArg[0] = 0;
        }

        sendCmd('COR', sendArg, 1);

        return;
    } else if (argCount == 1) {
        if (args[0] == 0) {
            dccsignal::SetBiDiCutout(false);
        } else if (args[1] == 0) {
            dccsignal::SetBiDiCutout(true);
        }
        return;
    } else {
        return;
    }
}

