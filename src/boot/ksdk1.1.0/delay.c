#include <assert.h>
#include "fsl_clock_manager.h"
#include "fsl_lptmr_hal.h"


#define BM_LPTMR_INSTANCE 1
#define BM_LPTMR_BASE LPTMR0_BASE

/* Microsecond delay using assembly commands */
void us_delay(unsigned int us_time)
{
	unsigned int i;

	for(i=0; i<us_time; i++) // 4 cycle +
	{
		asm volatile("nop");
        asm volatile("nop");    // 2 cycle +
        asm volatile("nop");
        asm volatile("nop");    // 2 cycle +
        asm volatile("nop");
        asm volatile("nop");    // 2 cycle +
		asm volatile("nop");
        asm volatile("nop");    // 2 cycle +
        asm volatile("nop"); 
		asm volatile("nop"); 	// 2 cycle = 16 cycle = 1us for 16MHz
	}
}