
#ifdef __cplusplus
extern "C" void initADCmodule(void);

extern "C" uint16_t adcCurrentReadEvent(void);
extern "C" uint16_t adcCurrentPeakRead(void);
extern "C" uint16_t adcVoltageReadEvent(void);

extern "C" void adcMsEvent(void);
extern "C" uint16_t readADCcachedValue(uint8_t ch);
#else
void initADCmodule(void);

uint16_t adcCurrentReadEvent(void);
uint16_t adcCurrentPeakRead(void);
uint16_t adcVoltageReadEvent(void);

void adcMsEvent(void);
uint16_t readADCcachedValue(uint8_t ch);
#endif