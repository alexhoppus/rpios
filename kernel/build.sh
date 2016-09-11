RPI_OS_BUILD_DIR=build
if [ -d $RPI_OS_BUILD_DIR ]; then
	rm -fr $RPI_OS_BUILD_DIR
fi

if [ -z $CROSS_COMPILE ]; then
	export CROSS_COMPILE=arm-none-eabi-
fi

mkdir $RPI_OS_BUILD_DIR
make all
mv *.o $RPI_OS_BUILD_DIR
mv kernel $RPI_OS_BUILD_DIR
$CROSS_COMPILE"objcopy" ./$RPI_OS_BUILD_DIR/kernel -O binary ./$RPI_OS_BUILD_DIR/kernel.bin
