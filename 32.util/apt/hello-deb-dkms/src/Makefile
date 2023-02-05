.PHONY: all clean

obj-m += hello_deb.o

kernelver ?= $(shell uname -r)

all:
	$(MAKE) -C /lib/modules/$(kernelver)/build M=$(CURDIR) modules
clean:
	$(MAKE) -C /lib/modules/$(kernelver)/build M=$(CURDIR) clean