CFLAGS=-O2 -fno-exceptions -fno-rtti -fno-stack-protector -ffreestanding -fpic -nostdlib -Wall -DVPBASE=0xf2000000 -DPBASE=0x3f000000 -DDEBUG
CC=$(CROSS_COMPILE)g++
EXECUTABLE=kernel

all: $(EXECUTABLE)

kernel: boot.o main.o uart.o lib.o mmu.o mm.o console.o debug.o sched.o
	$(CC) -mapcs -T linker.ld $(CFLAGS) boot.o main.o uart.o lib.o mmu.o mm.o console.o debug.o sched.o $(USERDIR)/app.blob -o $(EXECUTABLE) 

boot.o: boot.S
	$(CC) $(CFLAGS) -c boot.S

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

uart.o: uart/uart.cpp
	$(CC) $(CFLAGS) -c uart/uart.cpp

lib.o: common/lib.cpp
	$(CC) $(CFLAGS) -c common/lib.cpp

mmu.o: mm/mmu.cpp
	$(CC) $(CFLAGS) -c mm/mmu.cpp

mm.o: mm/mm.cpp
	$(CC) $(CFLAGS) -c mm/mm.cpp

debug.o: debug/debug.cpp
	$(CC) $(CFLAGS) -c debug/debug.cpp

console.o: console/console.cpp
	$(CC) $(CFLAGS) -c console/console.cpp

sched.o: sched/sched.cpp
	$(CC) $(CFLAGS) -c sched/sched.cpp
clean:
	rm *o kernel.bin kernel.elf
