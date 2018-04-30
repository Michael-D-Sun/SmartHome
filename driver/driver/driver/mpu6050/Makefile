#KERNELDIR := /lib/modules/$(shell  uname  -r)/build
KERNELDIR = /home/will/smarthome/kernel/linux-3.14-fs4412/

.PHONY:all
all:test app

test:
	make -C $(KERNELDIR)  M=$(shell pwd)    modules
app:
	arm-none-linux-gnueabi-gcc app.c -o app
	sudo cp app *.ko /rootfs

clean:
	make -C $(KERNELDIR)  M=$(shell pwd)    clean
	rm app

obj-m=smart_home_mpu6050.o



