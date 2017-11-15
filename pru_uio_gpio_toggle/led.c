#include <stdio.h>
#include <prussdrv.h>
#include <pruss_intc_mapping.h>
#define PRU_NUM 0

void main(void)
{
	tpruss_intc_initdata pruss_intc_initdata = PRUSS_INTC_INITDATA;
	prussdrv_init();
	prussdrv_open(PRU_EVTOUT_0);
	prussdrv_pruintc_init(&pruss_intc_initdata);
	prussdrv_exec_program(PRU_NUM, "./led.bin");
	int n = prussdrv_pru_wait_event(PRU_EVTOUT_0);
	prussdrv_pru_disable(PRU_NUM);
	prussdrv_exit();
}
