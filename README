This is early version of rpios kernel.
Now it includes uart bootloader and kernel itself.

HOW TO USE:
Be sure that eabi toolchain is installed with g++ support.
Particularly we use arm-none-eabi-... cross toolchain

$ cd ./uart_bootloader
$ ./build.sh
This will build a uart_bootloader. Move the kernel.bin image
to SD card boot partition and substitute kernel7.img for RPI2
or kernel.img for RPI. If you use RPI (not RPI2) you should
change flag -DRPI2 to -DRPI in build script.
After setup bootloader should print:
: Waiting for image data on UART
$ cd ./kernel
$ ./build.sh
Use send.sh script to communicate with uart bootloader and load
a kernel.
$ ./send.sh /dev/ttyUSB0
You should see something like:
: Waiting for image data on UART
: Data transfer detected, downloading the image ...
: Image size (bytes): 4324

: Data transfer finished, giving up control to kernel
: Hi iam kernel





