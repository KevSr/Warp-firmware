#include <stdlib.h>
#include <sys/time.h>
#include <stdio.h>

#include "fsl_misc_utilities.h"
#include "fsl_device_registers.h"
#include "fsl_gpio_driver.h"
#include "fsl_rtc_driver.h"
#include "fsl_clock_manager.h"
#include "fsl_power_manager.h"
#include "fsl_mcglite_hal.h"
#include "fsl_gpio_hal.h"
#include "fsl_port_hal.h"

#include "delay.h"
#include "gpio_pins.h"
#include "SEGGER_RTT.h"
#include "warp.h"
#include "devHPA17F.h"
#include "devSSD1331.h"



enum
{
    kHCSR04PinTRIG       = GPIO_MAKE_PIN(HW_GPIOB, 10),
    kHCSR04PinECHO       = GPIO_MAKE_PIN(HW_GPIOA, 5),
    kHCSR04PinVIBR       = GPIO_MAKE_PIN(HW_GPIOA, 6),
};
const gpio_input_pin_user_config_t inputPin[] = {
     {
         .pinName = kHCSR04PinECHO,
         .config.isPullEnable = true,
         .config.pullSelect = kPortPullDown,
         .config.isPassiveFilterEnabled = false,
         .config.interrupt = kPortIntDisabled,
     },
     {
        // Note: This pinName must be defined here to indicate the end of the array.
        .pinName = GPIO_PINS_OUT_OF_RANGE,
     }
};
const gpio_output_pin_user_config_t	outputPin[] = {
	{
		.pinName = kHCSR04PinTRIG,
		.config.outputLogic = 0,
		.config.slewRate = kPortSlowSlewRate,
		.config.driveStrength = kPortLowDriveStrength,
	},
    {
        // Note: This pinName must be defined here to indicate the end of the array.
        .pinName = GPIO_PINS_OUT_OF_RANGE,
    }

};

void devHCSR04init()
{
	/*
	 *	Override Warp firmware's use of these pins.
	 */

	GPIO_DRV_Init(inputPin, outputPin);
    PORT_HAL_SetMuxMode(PORTA_BASE, 6u, kPortMuxAsGpio);
    return;
}

volatile uint32_t value;
unsigned short tick=0, pulse_check=0, pulse_end=0;
WarpStatus
readSensorHCSR04()
{
	GPIO_DRV_SetPinOutput(kHCSR04PinTRIG);
    // 10 us delay to trigger HCSR04 sensor
    us_delay(10);
    GPIO_DRV_ClearPinOutput(kHCSR04PinTRIG);
    // wait for the pulse to be shot
    us_delay(100);
    do
    {   
        if (value == 1)
        {
            unsigned short pulse_tick;
            tick = 0;
            do
            {
                pulse_tick = tick;
                if(value == 0)
                {
                    pulse_end = pulse_tick;
                    SEGGER_RTT_printf(0, "%d\n", pulse_end);
                    return kWarpStatusOK;
                }
                value = GPIO_DRV_ReadPinInput(kHCSR04PinECHO);
                us_delay(20);
                tick++;

            } while (tick < 1900); // Over 38ms means that nothing has been detected
            
        }
        value = GPIO_DRV_ReadPinInput(kHCSR04PinECHO);
        us_delay(20);
        tick++;
    }while(tick < 1900);

    return kWarpStatusCommsError;

}

/* 
 * devSSD1331 codes were for debugging purposes.
 */
void printSensorDataHCSR04()
{
    WarpStatus	key;
    float distance;
    char decimal, nondec;
    SEGGER_RTT_printf(0, "%d\n", GPIO_DRV_GetPinDir(kHCSR04PinTRIG));
    
	OSA_TimeDelay(100);
	SEGGER_RTT_printf(0, "%d\n", GPIO_DRV_GetPinDir(kHCSR04PinECHO));
	OSA_TimeDelay(100);
    devHPA17Finit();
//    devSSD1331init();
    do
    {
        key = readSensorHCSR04();
        distance =(331.5*(pulse_end*0.01));
        nondec = distance;
        decimal = distance - nondec;
        if(key == kWarpStatusOK){
            if(distance < 30){
                pwmstart();
                GPIO_DRV_SetPinOutput(kHCSR04PinVIBR);
                
//                devSSD1331Red();
            }
            else if(distance < 60 && distance >= 30){
                pwmstart();
//                devSSD1331Yellow();
            }
            else{
                pwmstop();
                GPIO_DRV_ClearPinOutput(kHCSR04PinVIBR);
//                devSSD1331Green();
            }
            SEGGER_RTT_printf(0, "%d.%d", nondec, decimal);
        }
        else{
            SEGGER_RTT_printf(0, "Out of range\n");
            pwmstop();
            GPIO_DRV_ClearPinOutput(kHCSR04PinVIBR);
        }
        //Wait for signals to settle down
        OSA_TimeDelay(30);
    }while(1);
    
}