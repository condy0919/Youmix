QEMU = qemu-system-i386

KERNEL = Youmix

CC = i686-elf-gcc
CXX = i686-elf-g++
AS = i686-elf-as

CXXFLAGS = -c -Wall -Wextra -ffreestanding -fno-exceptions -fno-rtti -ggdb -gstabs+ -std=c++11
ASFLAGS = -g --gstabs

CPP_SRCS = $(shell find . -iname "*.cpp")
CPP_OBJS = $(patsubst %.cpp, %.o, $(CPP_SRCS))
AS_SRCS = $(shell find . -iname "*.s")
AS_OBJS = $(patsubst %.s, %.o, $(AS_SRCS))

# for global constructor
CRTI_SRC = $(shell find . -iname "crti.s")
CRTI_OBJ = $(patsubst %.s, %.o, $(CRTI_SRC))
CRTBEGIN_OBJ = $(shell $(CXX) $(CXXFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ = $(shell $(CXX) $(CXXFLAGS) -print-file-name=crtend.o)
CRTN_SRC = $(shell find . -iname "crtn.s")
CRTN_OBJ = $(patsubst %.s, %.o, $(CRTN_SRC))

OBJS = $(filter-out $(CRTI_OBJ) $(CRTN_OBJ), $(AS_OBJS)) $(CPP_OBJS)

OBJ_LINK_LIST = $(CRTI_OBJ) $(CRTBEGIN_OBJ) $(OBJS) $(CRTEND_OBJ) $(CRTN_OBJ)
INTERNAL_OBJS = $(CRTI_OBJ) $(OBJS) $(CRTN_OBJ)


LINK: $(OBJ_LINK_LIST)
	@echo -e "\033[31mLINK\033[0m kernel file"
	$(CXX) -T tools/linker.ld -o $(KERNEL) $(OBJ_LINK_LIST) -ffreestanding -nostdlib -lgcc

.cpp.o:
	@echo -e "\033[31mCompile\033[0m C++ source code $< ..."
	$(CXX) $(CXXFLAGS) $< -o $@

.s.o:
	@echo -e "\033[31mCompile\033[0m assembler source code $< ..."
	$(AS) $(ASFLAGS) $< -o $@

clean:
	$(RM) $(INTERNAL_OBJS) $(KERNEL)

update_image:
	sudo mount floppy.img /mnt
	sudo cp $(KERNEL) /mnt/kernel
	sudo umount /mnt

qemu:
	$(QEMU) -kernel $(KERNEL)

qemu-floppy:
	$(QEMU) -fda floppy.img -boot a

debug:
	$(QEMU) -S -s -kernel $(KERNEL) &
	cgdb -x tools/gdbinit 

debug-floppy:
	$(QEMU) -S -s -fda floppy.img -boot a &
	cgdb -x tools/gdbinit
