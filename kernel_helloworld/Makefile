obj-m+=hello.o # obj-m defines a LKM goal

# -C switches directory to the kernel directory
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean