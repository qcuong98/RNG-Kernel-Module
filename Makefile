obj-m+=kernel_module.o

all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
	$(CC) user_space.c -o main
clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
	rm main