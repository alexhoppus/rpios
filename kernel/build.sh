RPI_OS_BUILD_DIR=build
export USERDIR=../user
if [ -d $RPI_OS_BUILD_DIR ]; then
	rm -fr $RPI_OS_BUILD_DIR
fi

if [ -z $CROSS_COMPILE ]; then
	export CROSS_COMPILE=arm-none-eabi-
fi

echo "Building user app"
cd $USERDIR
./build.sh
cd -
echo "Building user app: done"

echo "Building kernel"
mkdir $RPI_OS_BUILD_DIR
make all
mv *.o $RPI_OS_BUILD_DIR
mv kernel $RPI_OS_BUILD_DIR
$CROSS_COMPILE"objcopy" ./$RPI_OS_BUILD_DIR/kernel -O binary ./$RPI_OS_BUILD_DIR/kernel.bin
ksize=$(ls -lah ./build/kernel.bin | awk {'print $5'})
echo "Building kernel: done, overall kernel size "$ksize
