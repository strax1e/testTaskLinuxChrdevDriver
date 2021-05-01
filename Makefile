moduleName = my_dummy_device_driver1
obj-m += $(addsuffix .o, $(moduleName))

all:
	make -C /lib/modules/`uname -r`/build/ M=$(PWD) modules
	
clean:
	make -C /lib/modules/`uname -r`/build M=$(PWD) clean
