#ifdef __cplusplus
extern "C" void initI2Cport(void);
extern "C" uint8_t writeI2Cpacket(uint8_t addr, uint8_t *packet, uint8_t length);
#else
void initI2Cport(void);
uint8_t writeI2Cpacket(uint8_t addr, uint8_t *packet, uint8_t length);
#endif