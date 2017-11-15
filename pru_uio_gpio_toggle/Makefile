CFLAGS += --std=gnu99 -O2 -Wall

CC := $(CC)
RM := $(RM)
PASM := pasm -DBUILD_WITH_PASM=1
DTC := dtc

.PHONY: all clean install

TARGETS := led led.bin prudaq-00A0.dtbo

all: $(TARGETS)

clean:
		$(RM) $(TARGETS) *.o
		
install: prudaq-00A0.dtbo
		install -v $^ /lib/firmware
		
%.bin: %.p
	$(PASM) -b $^
	
led: led.o
	$(CC) -o $@ $^ -l prussdrv
		
		
%.dtbo: %.dts
		$(DTC) -I dts -b0 -O dtb -@ -o $@ $^