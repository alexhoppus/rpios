CFLAGS=-O2 -fno-exceptions -fno-rtti -fno-stack-protector -ffreestanding -fpic -nostdlib -Wall
CC=$(CROSS_COMPILE)g++

all:
	$(CC) app1.c appboot.S usys.S -o app1 -T linker.ld $(CFLAGS)
	$(CC) app2.c appboot.S usys.S -o app2 -T linker.ld $(CFLAGS)
clean:
	rm -f *.o app app.blob app.blob.bin
