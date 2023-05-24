
/*
#ifdef __cplusplus
extern "C" void initDCCsignalPort(void);
extern "C" void dcc_railcom_cutout(void);
extern "C" void dcc_send_packet(uint8_t *array, uint8_t length, uint8_t cycle);

extern "C" void setDCCpowerStat(bool stat);
extern "C" void toggleDCCpowerStat(void);
extern "C" bool dcc_railcom_cutout_running(void);
#else
void initDCCsignalPort(void);
void dcc_railcom_cutout(void);
void dcc_send_packet(uint8_t *array, uint8_t length, uint8_t cycle);

void setDCCpowerStat(bool stat);
void toggleDCCpowerStat(void);
bool dcc_railcom_cutout_running(void);
#endif
*/

class dccport
{
public:
    static void init();
    static void event();
    static void dcc_railcom_cutout(void);
    static void dcc_send_packet(uint8_t *array, uint8_t length, uint8_t cycle);

    static void setPowerStat(bool stat);
    static void togglePowerStat(void);
    static bool getPowerStat(void);
    static bool dcc_railcom_cutout_running(void);

    static void setProgmode(bool stat);

    static bool powerInitCheck(void);

private:
    static uint8_t powerStat;
    static bool execCutout;

    static void _dcc_send_pulse(uint16_t waitTimeR, uint16_t waitTimeL);
    static void dcc_send_pulse(uint8_t value);
    static void dcc_send_preamble(void);

    static bool progMode;

    static uint16_t powerInCount;
};

#define DCC_POWER_STAT_OFF 0
#define DCC_POWER_STAT_ON 1
#define DCC_POWER_STAT_ANALOG 2

#define DCC_POWER_IN_INIT_COUNT 1000