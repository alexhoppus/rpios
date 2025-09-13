set architecture armv7
target remote localhost:1234
add-symbol-file /home/alexhoppus/rpios/kernel/build/kernel 0xc0032000
add-symbol-file /home/alexhoppus/rpios/kernel/build/kernel 0x32000
b _start
