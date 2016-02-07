RPI_OS_BUILD_DIR=build
if [ -d $RPI_OS_BUILD_DIR ]; then
	rm -fr $RPI_OS_BUILD_DIR
fi
mkdir $RPI_OS_BUILD_DIR
arm-none-eabi-g++ -ffreestanding -fpic -c boot.S -o ./$RPI_OS_BUILD_DIR/boot.o
arm-none-eabi-g++ -ffreestanding -fpic -c main.cpp -o ./$RPI_OS_BUILD_DIR/main.o -DRPI2
arm-none-eabi-g++ -ffreestanding -fpic -c uart/uart.cpp -o ./$RPI_OS_BUILD_DIR/uart.o -DRPI2
arm-none-eabi-g++ -ffreestanding -fpic -c common/lib.cpp -o ./$RPI_OS_BUILD_DIR/lib.o -DRPI2
arm-none-eabi-g++ -T linker.ld -o ./$RPI_OS_BUILD_DIR/kernel.elf -ffreestanding -O2 -nostdlib ./$RPI_OS_BUILD_DIR/boot.o ./$RPI_OS_BUILD_DIR/main.o ./$RPI_OS_BUILD_DIR/uart.o ./$RPI_OS_BUILD_DIR/lib.o
arm-none-eabi-objcopy ./$RPI_OS_BUILD_DIR/kernel.elf -O binary ./$RPI_OS_BUILD_DIR/kernel.bin
#sudo cp build/kernel.bin /media/alexhoppus/boot/kernel7.img
#sudo umount /media/alexhoppus/boot

