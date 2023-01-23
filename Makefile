C_SRC := $(wildcard src/c-code/*.c)
C_OBJS := $(patsubst src/c-code/%.c, bin/%.o,$(C_SRC))

ASM_SRC := $(wildcard src/assembly/*.asm)
ASM_OBJS := $(patsubst src/assembly/%.asm, bin/%.o,$(ASM_SRC))

CC=x86_64-linux-gnu-gcc
LD=x86_64-linux-gnu-ld

all: build

create-bin:
	@mkdir -p bin

build: create-bin $(ASM_OBJS) $(C_OBJS)
	$(LD) -n -o bin/kernel.bin -T linker.ld  $(C_OBJS) $(ASM_OBJS)
	cp bin/kernel.bin iso/boot/kernel.bin
	grub-mkrescue -o bin/kernel.iso iso

bin/interrupts.o: src/c-code/interrupts.c
	x86_64-linux-gnu-gcc -mno-red-zone -mgeneral-regs-only -ffreestanding -c $^ -o $@

bin/%.o: src/%.c
	x86_64-linux-gnu-gcc -c -ffreestanding $< -o $@ -I./src/include

build-debug: build create-debug-info

create-debug-info:
	objcopy --only-keep-debug bin/kernel.bin bin/kernel.sym

bin/%.o: src/c-code/%.c /
	$(CC) -g -DDEBUG -ggdb -Wall -c -ffreestanding $< -o $@ -I./src/include
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
	rm -f shell

docker-create:
	docker build --network=host -t almorina-builder .  
docker-build:
	docker run -v $(shell pwd):/mnt -w /mnt -it almorina-builder make
	sudo chmod -R g+w bin/kernel.iso bin/*.o
