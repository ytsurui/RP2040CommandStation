
#include "pico/stdlib.h"
#include <stdio.h>

#include "mt40bus.h"

#include "../wconfig/wconfig.h"

void mt40busCtrl::execCmdWCFG(uint32_t *args, uint8_t argCount)
{
    if (argCount == 1) {
        // Wireless Config Request
        if (args[0] == WIRELESS_CONFIG_MODULE_TYPE_TWELITE_UART) {
            // TWE LITE UARTの問い合わせだった場合、Application IDを返す

            uint32_t respArgs[2];
            respArgs[0] = args[0];
            respArgs[1] = wirelessConfig::twelite_app_id;
            sendCmd('WCFG', respArgs, 2, true);

            return;
        }
    }
}