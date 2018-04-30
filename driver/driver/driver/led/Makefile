#KERNELDIR := /lib/modules/$(shell  uname  -r)/build
KERNELDIR = /home/will/smarthome/kernel/linux-3.14-fs4412/

all: test app

test:
	make -C $(KERNELDIR)  M=$(shell pwd)    modules
	sudo cp *.ko /rootfs

app:app.o
	arm-none-linux-gnueabi-gcc app.o -o app
	sudo cp app /rootfs

app.o:app.c
	arm-none-linux-gnueabi-gcc -c -o app.o app.c
	


clean:
	make -C $(KERNELDIR)  M=$(shell pwd)    clean
	rm app

obj-m=smart_home_led.o



