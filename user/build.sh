if [ -z $CROSS_COMPILE ]; then
	export CROSS_COMPILE=arm-none-eabi-
fi

make all
$CROSS_COMPILE"objcopy" -I binary -O elf32-littlearm -B arm app app.blob

