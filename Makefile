#C_SRCS = $(shell find . -iname "*.c")
#C_OBJS = $(patsubst %.c, %.o, $(C_SRCS))
CPP_SRCS = $(shell find . -iname "*.cpp")
CPP_OBJS = $(patsubst %.cpp, %.o, $(CPP_SRCS))
ASM_SRCS = $(shell find . -iname "*.s")
ASM_OBJS = $(patsubst %.s, %.o, $(ASM_SRCS))

QEMU = qemu-system-i386

KERNEL = Youmix

#CC = gcc
CXX = g++
#LD = ld
ASM = nasm

#CFLAGS = -c -Wall -m32 -ggdb -gstabs+ -nostdlib -fno-builtin -fno-stack-protector -I include -std=c99
CXXFLAGS = -c -Wall -m32 -ggdb -gstabs+ -nostdlib -fno-builtin -fno-stack-protector -I include -ffreestanding -fno-rtti -fno-exceptions -std=c++11
#LDFLAGS = -T tools/kernel.ld -m elf_i386 -nostdlib
ASMFLAGS = -f elf -g -F stabs

all: $(ASM_OBJS) $(CPP_OBJS) link update_image

.cpp.o:
	@echo Compile C++ source code $< ...
	$(CXX) $(CXXFLAGS) $< -o $@

.s.o:
	@echo Compile assembler source code $< ...
	$(ASM) $(ASMFLAGS) $<

link:
	@echo Link kernel file
#$(LD) $(LDFLAGS) $(ASM_OBJS) $(C_OBJS) -o $(KERNEL)
	$(CXX) -T tools/kernel.ld -m32 -nostdlib $(ASM_OBJS) $(CPP_OBJS) -o $(KERNEL)

.PHONY: clean
clean:
	$(RM) $(ASM_OBJS) $(CPP_OBJS) $(KERNEL)

.PHONY: update_image
update_image:
	sudo mount floppy.img /mnt
	sudo cp $(KERNEL) /mnt/kernel
	sleep 1
	sudo umount /mnt/

.PHONY: mount_image
mount_image:
	sudo mount floppy.img /mnt/

.PHONY: umount_image
umount_image:
	sudo umount /mnt/

.PHONY: qemu
qemu:
	$(QEMU) -fda floppy.img -boot a

.PHONY: bochs
bochs:
	bochs -f tools/bochsrc.txt

.PHONY: debug
debug:
	$(QEMU) -S -s -fda floppy.img -boot a &
	sleep 1
	cgdb -x tools/gdbinit
