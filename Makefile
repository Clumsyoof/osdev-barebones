TARGET  := --target=i686-elf
CC      := clang
LD      := ld.lld
CFLAGS  := $(TARGET) -std=gnu99 -ffreestanding -fno-pic -fno-pie \
           -mno-sse -mno-sse2 -mno-mmx -fno-stack-protector \
           -fcf-protection=none -O2 -Wall -Wextra
LDFLAGS := -m elf_i386 -T linker.ld

OBJS := boot.o kernal.o
BIN  := myos.bin
ISO  := myos.iso

.PHONY: all run iso run-iso check clean

all: $(BIN)

$(BIN): $(OBJS) linker.ld
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(CC) $(TARGET) -c $< -o $@

check: $(BIN)
	grub-file --is-x86-multiboot $(BIN) && echo "multiboot ok"

run: $(BIN)
	qemu-system-x86_64 -kernel $(BIN)

iso: $(BIN) grub.cfg
	mkdir -p isodir/boot/grub
	cp $(BIN) isodir/boot/$(BIN)
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) isodir

run-iso: iso
	qemu-system-x86_64 -cdrom $(ISO)

clean:
	rm -f $(OBJS) $(BIN) $(ISO) qemu.log
	rm -rf isodir/boot/$(BIN)
