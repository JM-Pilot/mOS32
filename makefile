OUTPUT = mOS32
CC = i686-elf-gcc
AS = i686-elf-gcc

BIN_DIR = bin
INC_DIR = include
SRC_DIR = src
C_FLAGS = -ffreestanding \
	  -nostdlib \
	  -O2 \
	  -Wall \
	  -Wextra \
	  -m32 \
	  -I $(INC_DIR)
LD_FLAGS = -ffreestanding \
	   -nostdlib \
	   -T src/kernel/entry/linker.ld

C_SRCS = $(shell find $(SRC_DIR) -iname "*.c")
AS_SRCS = $(shell find $(SRC_DIR) -iname "*.s")

C_OBJS = $(patsubst $(SRC_DIR)/%.c, $(BIN_DIR)/%.c.o, $(C_SRCS))
AS_OBJS = $(patsubst $(SRC_DIR)/%.s, $(BIN_DIR)/%.s.o, $(AS_SRCS))
OBJS = $(C_OBJS) $(AS_OBJS)

$(BIN_DIR)/%.c.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(C_FLAGS) -c $< -o $@

$(BIN_DIR)/%.s.o: $(SRC_DIR)/%.s
	mkdir -p $(dir $@)
	$(AS) -c $< -o $@

$(BIN_DIR)/kernel/misc/ter-u16n.o: $(SRC_DIR)/kernel/misc/ter-u16n.psf
	mkdir -p $(dir $@)
	objcopy -O elf32-i386 -B i386 -I binary $< $@
	readelf -sW $@

OBJS += $(BIN_DIR)/kernel/misc/ter-u16n.o

$(BIN_DIR)/$(OUTPUT)_kernel.bin: $(OBJS)
	$(CC) $(LD_FLAGS) $^ -o $@
	if grub-file --is-x86-multiboot $@; then \
		echo [LOG] MULTIBOOT CONFIRMED; \
	else \
		echo [LOG] MULTIBOOT NOT CONFIRMED; \
		exit 1; \
	fi;

$(BIN_DIR)/$(OUTPUT)_disk.img:
	dd if=/dev/zero of=$@ bs=512 count=131072
	mkfs.fat -F 32 $@
	mkdir -p $(BIN_DIR)/mnt
	sudo mount -o loop,uid=$(shell id -u),gid=$(shell id -g) $@ $(BIN_DIR)/mnt
	echo "Hello World" > $(BIN_DIR)/mnt/HELLO.TXT
	echo " " > $(BIN_DIR)/mnt/WORLD.TXT
	sudo umount $(BIN_DIR)/mnt
# idk i didnt read the fat document properly but exanging HELLO.TXT and WORLD.txt
# changes the ls order, it doesnt order it Alphabetically
$(BIN_DIR)/$(OUTPUT).iso: $(BIN_DIR)/$(OUTPUT)_kernel.bin
	mkdir -p $(BIN_DIR)/iso_build/boot/grub
	cp $(SRC_DIR)/kernel/entry/grub.cfg $(BIN_DIR)/iso_build/boot/grub/
	cp $< $(BIN_DIR)/iso_build/boot
	grub-mkrescue -o $@ $(BIN_DIR)/iso_build

run: $(BIN_DIR)/$(OUTPUT).iso $(BIN_DIR)/$(OUTPUT)_disk.img
	qemu-system-i386 -cdrom $(BIN_DIR)/$(OUTPUT).iso \
	 	-drive file=$(BIN_DIR)/$(OUTPUT)_disk.img,format=raw \
		-boot d \
		-no-reboot -no-shutdown -d int,cpu_reset \
		-D $(BIN_DIR)/QEMU_LOG.txt

clean:
	rm -rf $(BIN_DIR)
