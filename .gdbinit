set architecture arm

echo + target remote localhost:1234\n
target remote localhost:1234

echo + symbol-file uart_bootloader/build/kernel.elf\n
symbol-file uart_bootloader/build/kernel.elf
