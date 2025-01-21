KERNEL_VERSION=5.4

# BEAR = bear --
DEBUG = y

ifeq ($(DEBUG),y)
	DEBUG_FLAGS = -O -g -DHELLO_DEBUG
else
	DEBUG_FLAGS = -O2
endif

EXTRA_CFLAGS += $(DEBUG_FLAGS)

ifneq ($(KERNELRELEASE),)
	obj-m = hello.o
else
KERNEL_DIR ?= ../../linux-$(KERNEL_VERSION)
PWD := $(shell pwd)

modules:
	$(BEAR) $(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) clean
endif
