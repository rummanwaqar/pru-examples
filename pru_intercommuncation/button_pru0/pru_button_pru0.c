/*
 * Switch is connected to P9.27 (pru0_r31_5) in active high
 * Sends interrupt using event 16 to PRU1
 */

#include <stdint.h>
#include <pru_cfg.h>
#include <pru_intc.h>
#include "resource_table_empty.h"

#define PRU0

volatile register uint32_t __R30;
volatile register uint32_t __R31;
/*
 *  PRU0 to PRU1 interrupt
 *  PRU cores can generate interrupts manually using events 16-31
 *  however these are mapped to bits [3:0] in the r31 register
 *  Additionally, we have to trigger bit 5 to strobe the interrupt
 */
#define PRU0_PRU1_EVT (16)
#define PRU0_PRU1_TRIGGER (__R31 = (PRU0_PRU1_EVT - 16) | (1 << 5))

// switch mask
#define SW1 (1<<5)

/*
 * int configuration
 */
void configIntc(void)
{
	// clear any pending PRU events
	__R31 = 0x00000000;

	// map event 16 to channel 1
	// channel map registers (CMRm) define the channel for each system event
	// there is one register per 4 events, therefore 16 CMR for 64 events
	// for event 16 we will use CMR4
	CT_INTC.CMR4_bit.CH_MAP_16 = 1;

	// map channel 1 to host 1
	// host map registers (HMRm) define the channel for each host event
	// one register per 4 channels, therefore 3 HMR for 10 channels
	// host 1 is connected to R31 bit 31 for PRU0 and PRU1
	CT_INTC.HMR0_bit.HINT_MAP_1 = 1;

	// 4.4.2.3.2 Interrupt Enabling in TRM
	// ensure event 16 is cleared
	// after servicing an event, event status has to be cleared
	// event N can be cleared by writing N into the system event status indexed clear register (SICR)
	CT_INTC.SICR = 16;

	// enable event 16
	// system events that are required to be propagated to host must be enabled
	// event N can be enabled by writing N in the system event enable indexed set register (EISR)
	CT_INTC.EISR = 16;

	// enable host interrupt 1
	// done by writing N to HIEISR register
	CT_INTC.HIEISR |= (1 << 0);

	// globally enable host interrupts
	// set EN bit (bit 0) in global enable register (GER) to 1
	CT_INTC.GER = 1;
}

void main(void)
{
	// Configure GPI/O as Mode 0 (Direct input)
	// bit 1-0
	// 00 - direct input mode
	// 01 - 16bit parallel capture mode
	// 10 - 28bit shift mode
	// 11 - Mii_rt mode
	CT_CFG.GPCFG0 = 0x0000;

	// clear all 16 output pins
	__R30 &= 0xFFFF0000;

	// configure interrupt
	configIntc();

	while(1)
	{
		// wait for SW1 to be pressed (active high)
		if((__R31 & SW1) == SW1)
		{
			// wait 500 ms for debounce
			__delay_cycles(100000000);
			// interrupt PRU1
			PRU0_PRU1_TRIGGER;
		}
	}
}
