CFLAGS=-O2 -fno-exceptions -fno-rtti -fno-stack-protector -ffreestanding -fpic -nostdlib -Wall
CC=$(CROSS_COMPILE)g++

all:
	$(CC) app.c appboot.S -o app -T linker.ld $(CFLAGS)
clean:
	rm -f *.o app app.blob app.blob.bin
