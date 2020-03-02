

#KERNELDIR=/lib/modules/`uname -r`/build
#KERNELDIR=/lib/modules/4.13.0-26-generic/build

KERNELDIR=/home/linux/kernel/kernel
PWD=$(shell pwd)

all:
	make -C $(KERNELDIR) M=$(PWD) modules

clean:
	make -C $(KERNELDIR) M=$(PWD) clean

obj-m +=pwm-jz.o


