obj-m = hello2.o
KERNEL_VERSION=5.4

all:
	echo $(OBJECTS)
	bear -- make -C ../../linux-$(KERNEL_VERSION) M=$(PWD) modules
clean:
	make -C ../../linux-$(KERNEL_VERSION) M=$(PWD) clean
