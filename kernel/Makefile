CPP_FILES := $(wildcard *.cpp)
ASM_FILES := $(wildcard *.S)
OBJ_FILES := $(addprefix build/,$(notdir $(CPP_FILES:.cpp=.o)))
OBJ_FILES += $(addprefix build/,$(notdir $(ASM_FILES:.S=.o)))
CC_FLAGS=-Os -fno-exceptions -fno-rtti -fno-stack-protector -fpic -Wall -DVPBASE=0xf2000000 -DPBASE=0x3f000000 -DDEBUG
LD_FLAGS=-mapcs -T linker.ld -ffreestanding -nostdlib
CC=$(CROSS_COMPILE)g++
EXECUTABLE=kernel

all: $(EXECUTABLE)

kernel: $(OBJ_FILES)
	$(CC) $(LD_FLAGS) -o $@ $^ ../$(USERDIR)/app1.blob ../$(USERDIR)/app2.blob
	mv kernel build/

build/%.o: %.cpp
	$(CC) $(CC_FLAGS) -c -o $@ $<

build/%.o: %.S
	$(CC) $(CC_FLAGS) -c -o $@ $<

clean:
	rm -f build/*
