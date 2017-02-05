KERNELDIR=kernel
RPI_OS_BUILD_DIR=build
if [ ! -d $KERNELDIR/$RPI_OS_BUILD_DIR ]; then
	echo "No build dir found"
	exit
fi

if [ -z $1 ]; then
	echo "Please specify the terminal, f.e. ./send.sh /dev/ttyUSB0"
	exit
fi

# Sending magic
echo -n "blabla" > $1
# dummy flush
sleep 0.1
# Sending size
ls -la $KERNELDIR/$RPI_OS_BUILD_DIR/kernel.bin | awk {'print $5'} > $1
# dummy flush
sleep 0.1
# Sending image
echo "Sending image..."
cat $KERNELDIR/$RPI_OS_BUILD_DIR/kernel.bin > $1
echo "Done"


