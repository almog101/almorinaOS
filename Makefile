C_SRC := $(wildcard src/*.c)
C_OBJS := $(patsubst src/%.c, bin/%.o,$(C_SRC))

ASM_SRC := $(wildcard src/assembly/*.asm)
ASM_OBJS := $(patsubst src/assembly/%.asm, bin/%.o,$(ASM_SRC))

all: build

create-bin:
	@mkdir -p bin

build: create-bin $(ASM_OBJS) $(C_OBJS)
	x86_64-linux-gnu-ld -n -o bin/kernel.bin -T linker.ld  $(C_OBJS) $(ASM_OBJS)
	cp bin/kernel.bin iso/boot/kernel.bin
	grub-mkrescue /usr/lib/grub/i386-pc -o bin/kernel.iso iso

bin/interrupts.o: src/interrupts.c
	x86_64-linux-gnu-gcc -mno-red-zone -mgeneral-regs-only -ffreestanding -c $^ -o $@

bin/%.o: src/%.c
	x86_64-linux-gnu-gcc -c -ffreestanding $< -o $@ -I./src/include

bin/%.o: src/assembly/%.asm
	nasm -f elf64 $< -o $@

run:
	qemu-system-x86_64 bin/kernel.iso

clean:
	rm -rf bin iso/boot/kernel.bin
