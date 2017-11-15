/*
 * led is connected to P8.44 (pru1_r30_3)
 */

#include <stdint.h>
#include <pru_cfg.h>
#include <pru_intc.h>
#include "resource_table_empty.h"

volatile register uint32_t __R30;
volatile register uint32_t __R31;

#define PRU1
// host 1 is mapped to bit 31 of R31
#define HOST1_MASK		(1<<31)
#define PRU0_PRU1_EVT	(16)
#define TOGGLE_LED		(__R30 ^= (1<<3))

void main(void)
{
	// GPIO mode - direct connect
	CT_CFG.GPCFG0 = 0x0000;

	// clear all output pins
	__R30 &= 0xFFFF0000;

	while(1)
	{
		// wait while host 1 is detected
		if(__R31 & HOST1_MASK)
		{
			TOGGLE_LED;
			// clear interrupt event
			CT_INTC.SICR = 16;

			// delay to ensure that the event is cleared in INTC
			__delay_cycles(5);	// 5 cycles is an overkill
		}
	}
}
