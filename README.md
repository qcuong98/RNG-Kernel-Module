THIS KERNEL MODULE IS USE FOR GENERATING RANDOM NUMBERS
CAN BE CALLED BY USER SPACE CODE WHEN USE: open("/dev/randomness_generator", O_RDONLY);

----------------------------------------------------------------
1.  Open Terminar in this directory
2.  Create files for kernel module  && user space run file: make
3.  Insert kernel module: sudo insmod kernel_module.ko
4.  Run user space code: sudo ./main
5.  Remove kernel module: sudo rmmod kernel_module
6.  Remove created files: make clean
