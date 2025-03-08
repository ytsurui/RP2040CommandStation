#include <string>
#include <sstream>
#include <cstring>

#include "pico/stdlib.h"
#include "wconfig.h"

#include "../../peripheral/uart_ctrl.h"
#include "../../peripheral/eventTimer.h"


#define WIRELESS_DETECTOR_TIMEOUT 1000

uint8_t wirelessConfig::wModuleType = 0;
uint16_t wirelessConfig::lastRecvTime = 0;

uint32_t wirelessConfig::eventTimeCounter = 0;


bool wirelessConfig::InitNeedFlag = false;


std::string wirelessConfig::inputBuffer;
std::string wirelessConfig::currentLine;


void configRecv(uint8_t rData)
{
    wirelessConfig::detectorRecv(rData);
}

void wirelessConfig::detectorRecv(uint8_t rData)
{
    inputBuffer += static_cast<char>(rData);
}

void wirelessConfig::detectProc()
{
    bool endFlag = false;

    eventTimeCounter = 1;

    while(1) {
        // 起動後、1秒何もせず待つ
        if (eventtimer::checkMS()) {
            eventTimeCounter++;
            if (eventTimeCounter >= 1000) {
                // タイムアウト
                break;
            }
        }
    }
    eventTimeCounter = 0;

    while(1) {
        if (eventtimer::checkMS()) {
            eventTimeCounter++;
            switch (eventTimeCounter) {
                case 100:
                case 400:
                    uartCtrl::getInstance(1)->send('+');
                    break;
                case 700:
                    uartCtrl::getInstance(1)->setRecvCallback(configRecv);
                    uartCtrl::getInstance(1)->send('+');
                    break;
                case 750:
                    uartCtrl::getInstance(1)->clearRecvCallback();
                    endFlag = true;
                    break;
            }       
        }
        //uartWireless::task();
        uartCtrl::getInstance(1)->task();

        if (endFlag) {
            // 受信文字列の解析処理
            InitNeedFlag = false;
            analyze();
            break;
        }
    }

    eventTimeCounter = 0;
    endFlag = false;
    while(1) {
        if (eventtimer::checkMS()) {
            eventTimeCounter++;
            switch (eventTimeCounter) {
                case 100:
                case 400:
                    uartCtrl::getInstance(1)->send('+');
                    break;
                case 700:
                    uartCtrl::getInstance(1)->send('+');
                    endFlag = true;
                    break;
            }
        }

        if (endFlag) break;
    }
}

void wirelessConfig::analyze()
{
    std::istringstream iss(inputBuffer);
    std::string line;
    bool first_line_processed = false;
    char *test = new char[15];

    bool testWriteFlag = false;

    while (std::getline(iss, line)) {
        if (!first_line_processed) {
            // 最初の行を処理
            std::string first_line = line;
            if (first_line.length() >= 6 &&
                first_line.find("--- CONFIG") != std::string::npos) {
                // "TWE UART APP"が含まれているか確認
                if (first_line.find("TWE UART APP") != std::string::npos) {
                    //contains_TWE_UART_APP = true;
                    first_line_processed = true;
                    wModuleType = WIRELESS_CONFIG_MODULE_TYPE_TWELITE_UART;

                    // バージョン情報を抽出（'V'から始まり、次の'/'まで）
                    size_t pos_v = first_line.find('V');
                    if (pos_v != std::string::npos) {
                        size_t pos_end = first_line.find('/', pos_v);
                        if (pos_end != std::string::npos) {
                            std::string ver = first_line.substr(pos_v, pos_end - pos_v);
                            strncpy(versionInfo, ver.c_str(), sizeof(versionInfo) - 1);
                            versionInfo[sizeof(versionInfo) - 1] = '\0';  // 文字列の終端を確保
                        }
                    }
                    // SIDを抽出（"SID=0x"の後の16進数値）
                    size_t pos_sid = first_line.find("SID=0x");
                    if (pos_sid != std::string::npos) {
                        pos_sid += 6; // "SID=0x"の後に移動
                        std::string sid_hex;
                        while (pos_sid < first_line.length() && isxdigit(first_line[pos_sid])) {
                            sid_hex += first_line[pos_sid];
                            pos_sid++;
                        }
                        std::istringstream iss_sid(sid_hex);
                        iss_sid >> std::hex >> wModuleAddr;
                    }
                } else if (first_line.find("TWE") != std::string::npos) {
                    // Unknown TWE-LITE apps
                    wModuleType = WIRELESS_CONFIG_MODULE_TYPE_TWELITE_UNKNOWN;
                }
                
            }
        } else {
            // すでにデバイス種類を特定済み
            // ここで設定値を取得する
            if (wModuleType == WIRELESS_CONFIG_MODULE_TYPE_TWELITE_UART) {
                std::string pref;
                pref = line.substr(0, 4);

                if (pref == " a: ") {
                    // ApplicationID
                    size_t pos = line.find("0x");
                    if (pos != std::string::npos) {
                        std::string app_id_hex = line.substr(pos + 2);
                        std::istringstream iss_app_id(app_id_hex);
                        iss_app_id >> std::hex >> twelite_app_id;
                    }
                } else if (pref == " m: ") {
                    // UART mode
                    size_t pos = line.find("(");
                    if (pos != std::string::npos) {
                        std::string mode = line.substr(pos + 1, 1);
                        if (mode == "D") {
                            // Transparent mode

                        } else {
                            // Other
                            InitNeedFlag = true;
                        }
                    }
                } else if (pref == " k: ") {
                    // Tx Trigger
                    size_t pos = line.find("(sep=");
                    if (pos != std::string::npos) {
                        std::string sep = line.substr(pos + 5);
                        pos = sep.find(",");
                        sep = sep.substr(0, pos);
                        if (sep == "0x0d0a") {
                            // crlf, config not needed
                        } else {
                            // other
                            InitNeedFlag = true;
                        }
                    }
                } else if (pref == " c: ") {
                    // Channels

                }

            }
        }
    }
}

uint8_t wirelessConfig::getModuleType()
{
    return wModuleType;
}



void wirelessConfig::sendConfigCharProc(const char *cmds)
{
    uint32_t i = 0;
    eventTimeCounter = 0;

    uartCtrl::getInstance(1)->evacuationCallback();

    while (1) {
        if (eventtimer::checkMS()) {
            eventTimeCounter++;
            if (eventTimeCounter >= 350) {
                uartCtrl::getInstance(1)->send(cmds[i]);
                i++;
                if (i >= strlen(cmds)) {
                    break;
                }
                eventTimeCounter = 0;
            }
        }
        uartCtrl::getInstance(1)->task();
    }

    uartCtrl::getInstance(1)->restoreCallback();
}
