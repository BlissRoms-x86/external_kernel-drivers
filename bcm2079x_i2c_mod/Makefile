#KERNEL_BUILD_DIR=/lib/modules/$(shell uname -r)/build
#KERNEL_BUILD_DIR=/sdcard/tpt10/linux-sunxi/
#KERNEL_BUILD_DIR=/sdcard/tpt10/linux-4.10.10/

# acpi-tpt10-battery-m += tpt10-bat.o acpi-tpt10-battery.o

obj-m += bcm2079x-i2c.o

all:
	make -C $(KERNEL_BUILD_DIR) M=$(PWD) modules

test:
	$(CC) -Wall -O2 -o bcm2079x-test bcm2079x-test.c

clean:
	make -C $(KERNEL_BUILD_DIR) M=$(PWD) clean
	@rm bcm2079x-test
