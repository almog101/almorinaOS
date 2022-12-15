C_SRC := $(wildcard src/*.c)
C_OBJS := $(patsubst src/%.c, bin/%.o,$(C_SRC))

ASM_SRC := $(wildcard src/assembly/*.asm)
ASM_OBJS := $(patsubst src/assembly/%.asm, bin/%.o,$(ASM_SRC))

all: build

create-bin:
	@mkdir -p bin

build: create-bin $(ASM_OBJS) $(C_OBJS)
	x86_64-elf-ld -n -o bin/kernel.bin -T linker.ld  $(C_OBJS) $(ASM_OBJS)
	cp bin/kernel.bin iso/boot/kernel.bin
	grub-mkrescue -o bin/kernel.iso iso

build-debug: build create-debug-info

create-debug-info:
	objcopy --only-keep-debug bin/kernel.bin bin/kernel.sym

bin/%.o: src/%.c
	x86_64-elf-gcc -g -DDEBUG -ggdb -Wall -c -ffreestanding $< -o $@ -I./src/include
bin/%.o: src/assembly/%.asm
	nasm -f elf64 -g -F dwarf $< -o $@

run:
	qemu-system-x86_64 bin/kernel.iso

run-debug:
	qemu-system-x86_64 -s -S bin/kernel.iso

run-gdb:
	gdb -tui -ex "target remote localhost:1234" -ex "set disassembly-flavor intel" -ex "symbol-file bin/kernel.sym"

clean:
	rm -rf bin iso/boot/kernel.bin



