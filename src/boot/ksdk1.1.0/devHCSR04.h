#ifndef WARP_BUILD_ENABLE_DEVHCSR04
#define WARP_BUILD_ENABLE_DEVHCSR04
#endif


WarpStatus  readSensorHCSR04();
void        printSensorDataHCSR04();
void        init_counter(uint8_t instance);
void counter1(uint8_t instance);
void counter2(uint8_t instance);
int         devHCSR04init(void);
void        deinit_trigger_source();