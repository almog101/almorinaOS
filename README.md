# AlmorinaOS - 64-bit Linux-Based Operating System

**AlmorinaOS** is a 64-bit operating system built using C and Assembly language. It aims to provide a minimalistic and educational platform for learning about operating system development.

## Features

- 64-bit architecture
- C and Assembly language programming
- Multiboot compliant
- Basic interrupt handling
- Grub bootloader integration
- Debugging support using QEMU and GDB


### Prerequisites

- GNU Make
- NASM (Netwide Assembler)
- GCC cross-compiler (x86_64-elf-gcc or x86_64-linux-gnu-gcc)
- QEMU (for emulation)
- GDB (for debugging, optional)
- Docker (optional, for containerized builds)

### Building

1. **Clone the repository:**

    ```bash
    git clone https://github.com/your-username/almorinaOS.git
    cd almorinaOS
    ```

2. **Build the OS:**

    ```bash
    make build
    ```

3. **Emulate the OS using QEMU:**

    ```bash
    make run
    ```
