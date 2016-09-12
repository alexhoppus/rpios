RPI_OS_BUILD_DIR=build
if [ -d $RPI_OS_BUILD_DIR ]; then
	rm -fr $RPI_OS_BUILD_DIR
fi
mkdir $RPI_OS_BUILD_DIR
VPBASE="-DVPBASE=0x3f000000"
OPS="-ffreestanding -fpic "$VPBASE
arm-none-eabi-g++ -c boot.S -o ./$RPI_OS_BUILD_DIR/boot.o $OPS
arm-none-eabi-g++ -c main.cpp -o ./$RPI_OS_BUILD_DIR/main.o $OPS
arm-none-eabi-g++ -c uart/uart.cpp -o ./$RPI_OS_BUILD_DIR/uart.o $OPS
arm-none-eabi-g++ -c common/lib.cpp -o ./$RPI_OS_BUILD_DIR/lib.o $OPS
arm-none-eabi-g++ -T linker.ld -o ./$RPI_OS_BUILD_DIR/kernel.elf -ffreestanding -O2 -nostdlib ./$RPI_OS_BUILD_DIR/boot.o ./$RPI_OS_BUILD_DIR/main.o ./$RPI_OS_BUILD_DIR/uart.o ./$RPI_OS_BUILD_DIR/lib.o
arm-none-eabi-objcopy ./$RPI_OS_BUILD_DIR/kernel.elf -O binary ./$RPI_OS_BUILD_DIR/kernel.bin

