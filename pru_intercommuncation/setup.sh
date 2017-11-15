# sets up variables
export PRU_CGT=/usr/share/ti/cgt-pru
export PRU_SSP=/home/debian/pru-software-support-package

# config
GPIO_SW=P9.27
GPIO_LED=P8.44
BUTTON_CODE=button_pru0
LED_CODE=led_pru1

echo "-Configuring pinmux"
	config-pin -a $GPIO_SW pruin
	config-pin -q $GPIO_SW
	config-pin -a $GPIO_LED pruout
	config-pin -q $GPIO_LED

echo "-Placing the firmware in PRU0"
	if [ ! -d "$BUTTON_CODE/gen" ]; then
		echo "*ERROR: gen directory does not exist for $BUTTON_CODE. Try running make"
		exit 1
	fi
	cp $BUTTON_CODE/gen/*.out /lib/firmware/am335x-pru0-fw
	
echo "-Placing the firmware in PRU1"
	if [ ! -d "$LED_CODE/gen" ]; then
		echo "*ERROR: gen directory does not exist for $LED_CODE. Try running make"
		exit 1
	fi
	cp $LED_CODE/gen/*.out /lib/firmware/am335x-pru1-fw

echo "-Rebooting"
	echo "Rebooting pru-core 0"
	echo "4a334000.pru0" > /sys/bus/platform/drivers/pru-rproc/unbind 2>/dev/null
	echo "4a334000.pru0" > /sys/bus/platform/drivers/pru-rproc/bind

	echo "Rebooting pru-core 1"
	echo "4a338000.pru1"  > /sys/bus/platform/drivers/pru-rproc/unbind 2> /dev/null
	echo "4a338000.pru1" > /sys/bus/platform/drivers/pru-rproc/bind