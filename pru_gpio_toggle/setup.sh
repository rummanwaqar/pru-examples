# sets up variables
export PRU_CGT=/usr/share/ti/cgt-pru
export PRU_SSP=/home/debian/pru-software-support-package

# config
GPIO_PIN=P9.25
PRU_CORE=0

echo "-Configuring pinmux"
	config-pin -a $GPIO_PIN pruout
	config-pin -q $GPIO_PIN

echo "-Placing the firmware"
	if [ ! -d "gen" ]; then
		echo "*ERROR: gen directory does not exist. Try running make"
		exit 1
	fi
	cp gen/*.out /lib/firmware/am335x-pru$PRU_CORE-fw

echo "-Rebooting"
	if [ $PRU_CORE -eq 0 ]
	then
		echo "Rebooting pru-core 0"
		echo "4a334000.pru0" > /sys/bus/platform/drivers/pru-rproc/unbind 2>/dev/null
		echo "4a334000.pru0" > /sys/bus/platform/drivers/pru-rproc/bind
	else
		echo "Rebooting pru-core 1"
		echo "4a338000.pru1"  > /sys/bus/platform/drivers/pru-rproc/unbind 2> /dev/null
		echo "4a338000.pru1" > /sys/bus/platform/drivers/pru-rproc/bind
	fi