CFLAGS=-O2 -fno-exceptions -fno-rtti -fno-stack-protector -ffreestanding -fpic -nostdlib -Wall
CC=$(CROSS_COMPILE)g++
EXECUTABLE=kernel

all: $(EXECUTABLE)

kernel: boot.o main.o uart.o lib.o mmu.o mm.o console.o
	$(CC) -mapcs -T linker.ld $(CFLAGS) boot.o main.o uart.o lib.o mmu.o mm.o console.o -o $(EXECUTABLE)

boot.o: boot.S
	$(CC) $(CFLAGS) -c boot.S

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp -DRPI2

uart.o: uart/uart.cpp
	$(CC) $(CFLAGS) -c uart/uart.cpp -DRPI2

lib.o: common/lib.cpp
	$(CC) $(CFLAGS) -c common/lib.cpp

mmu.o: mm/mmu.cpp
	$(CC) $(CFLAGS) -c mm/mmu.cpp

mm.o: mm/mm.cpp
	$(CC) $(CFLAGS) -c mm/mm.cpp -DRPI2

console.o: console/console.cpp
	$(CC) $(CFLAGS) -c console/console.cpp

clean:
	rm *o kernel.bin kernel.elf
