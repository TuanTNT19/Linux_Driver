# Hello world kernel module

EXTRA_CFLAGS = -Wall
obj-m = main.o

KDIR = /lib/modules/`uname -r`/build

all:
	make -C $(KDIR) M=`pwd` modules

clean:
	make -C $(KDIR) M=`pwd` clean
change:
	sudo chmod 0666 /dev/my_device
rm:
	sudo rmmod main.ko
view:
	dmesg | tail
cat:
	cat /dev/my_device
run:
	sudo insmod main.ko

