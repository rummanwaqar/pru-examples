# Beaglebone PRU Examples

## Kernel: Hello World
Creates a kernel module hello world equivalent.

## PRU

### pru_gpio_toggle

Toggles P9_25 using PRU 0. Includes a timed delay. Uses remoteproc

### pru_uio_gpio_toggle

Toggles P9_25 using PRU 0. Includes a timed delay. Uses UIO

### pru_intercommunication

Demonstrates PRU multi-core communication by reading a button switch on PRU0 GPI & toggles LED on PRU1 GPO. Uses remoteproc. Not working. 