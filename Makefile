obj-m += hid-hvgamenote.o
all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	zstd -f hid-hvgamenote.ko
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
install:
	cp $(shell pwd)/hid-hvgamenote.ko.zst /lib/modules/$(shell uname -r)/kernel/drivers/hid/
	depmod -a
uninstall:
	rm $(shell pwd)/hid-hvgamenote.ko.zst /lib/modules/$(shell uname -r)/kernel/drivers/hid/hid-hvgamenote.ko.zst
	depmod -a
dkms:
	make -C $(INCLUDEDIR) M=$(PWD) modules

