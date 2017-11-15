/*
 * Toggle P9_25 LED on PRU 0 it's pin 7
 */

#include <stdint.h>
#include <pru_cfg.h>
#include "resource_table_empty.h"

volatile register uint32_t __R30;
volatile register uint32_t __R31;

void main(void)
{
	// clear SYSCFG[STANDBY_INIT] to enable OCP master port
	CT_CFG.SYSCFG_bit.STANDBY_INIT = 0;

	volatile uint32_t gpio_pin_mask = (1<<7);

	while(1)
	{
		__R30 ^= gpio_pin_mask;
		/*
		 * clock speed is 200MHz
		 * wait for 0.5 secs
		 * cycles = 5 * 200 * 10e6
		 */
		__delay_cycles(100000000);
	}
}
