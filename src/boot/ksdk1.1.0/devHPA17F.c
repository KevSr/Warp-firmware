#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "fsl_misc_utilities.h"
#include "fsl_device_registers.h"
#include "fsl_i2c_master_driver.h"
#include "fsl_spi_master_driver.h"
#include "fsl_tpm_driver.h"
#include "fsl_tpm_hal.h"
#include "fsl_rtc_driver.h"
#include "fsl_clock_manager.h"
#include "fsl_power_manager.h"
#include "fsl_mcglite_hal.h"
#include "fsl_port_hal.h"
#include "fsl_smc_hal.h"
#include "fsl_pmc_hal.h"

#include "SEGGER_RTT.h"
#include "gpio_pins.h"
#include "warp.h"

#include "devHPA17F.h"

#define TPM_INSTANCE    (0U)
#define TPM_CHANNEL     (0U)



const tpm_general_config_t tpmConfig = {
	  .isDBGMode = false,
    .isGlobalTimeBase = false,
    .isTriggerMode = false,
    .isStopCountOnOveflow = false,
    .isCountReloadOnTrig = false,
	  .triggerSource = 0,
};

volatile tpm_pwm_param_t tpmParameter = {
		.mode = kTpmEdgeAlignedPWM,
		.edgeMode = kTpmHighTrue,
		.uFrequencyHZ = 1000,
		.uDutyCyclePercent = 50,
};
enum
{
	kHPA17FPinPWM		= GPIO_MAKE_PIN(HW_GPIOB, 11),
};

int devHPA17Finit(void)
{
  PORT_HAL_SetMuxMode(PORTB_BASE, 11u, kPortMuxAlt2);

	//PWM initialisation
  TPM_DRV_Init(TPM_INSTANCE, (tpm_general_config_t *)&tpmConfig);
	TPM_DRV_SetClock(TPM_INSTANCE, kTpmClockSourceModuleMCGIRCLK, kTpmDividedBy1);
	return 0;
}

int pwmstart(void)
{
  if(!TPM_DRV_PwmStart(TPM_INSTANCE, (tpm_pwm_param_t *)&tpmParameter, TPM_CHANNEL))
	  {
		  SEGGER_RTT_printf(0, "PWM FAILED\n");
	  }
  
  return 0;
}

int pwmstop(void)
{
  TPM_DRV_PwmStop(TPM_INSTANCE, (tpm_pwm_param_t *)&tpmParameter, TPM_CHANNEL);

  return 0;
}