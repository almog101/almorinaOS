#pragma once
#include <stdint.h>
#include <stdbool.h>

#define ELF_NIDENT	16

typedef uint16_t elf32_half;	// Unsigned half int
typedef uint32_t elf32_off;	    // Unsigned offset
typedef uint32_t elf32_addr;	// Unsigned address
typedef uint32_t elf32_word;	// Unsigned int
typedef int32_t  elf32_sword;	// Signed int

#define ELFMAG0	0x7F // e_ident[EI_MAG0]
#define ELFMAG1	'E'  // e_ident[EI_MAG1]
#define ELFMAG2	'L'  // e_ident[EI_MAG2]
#define ELFMAG3	'F'  // e_ident[EI_MAG3]
 
#define ELFDATA2LSB	(1)  // Little Endian
#define ELFCLASS32	(1)  // 32-bit Architecture

#define EM_386		(3)  // x86 Machine Type
#define EV_CURRENT	(1)  // ELF Current Version

enum Elf_Ident 
{
	EI_MAG0		    = 0, // 0x7F
	EI_MAG1		    = 1, // 'E'
	EI_MAG2		    = 2, // 'L'
	EI_MAG3		    = 3, // 'F'
	EI_CLASS	    = 4, // Architecture (32/64)
	EI_DATA		    = 5, // Byte Order
	EI_VERSION	    = 6, // ELF Version
	EI_OSABI	    = 7, // OS Specific
	EI_ABIVERSION	= 8, // OS Specific
	EI_PAD		    = 9  // Padding
};

enum Elf_Type 
{
	ET_NONE		= 0, // Unkown Type
	ET_REL		= 1, // Relocatable File
	ET_EXEC		= 2  // Executable File
};

// the begining of every elf file
typedef struct 
{
	uint8_t		e_ident[ELF_NIDENT];
	elf32_half	e_type;
	elf32_half	e_machine;
	elf32_word	e_version;
	elf32_addr	e_entry;
	elf32_off	e_phoff;
	elf32_off	e_shoff;
	elf32_word	e_flags;
	elf32_half	e_ehsize;
	elf32_half	e_phentsize;
	elf32_half	e_phnum;
	elf32_half	e_shentsize;
	elf32_half	e_shnum;
	elf32_half	e_shstrndx;
} elf32_ehdr;

bool elf_check_supported(elf32_ehdr* hdr);
void* elf_load_file(void* file);

/**
==============
File Structure
==============

< basically a binary file >

sections
--------
TEXT:               the code
DATA:               global table, variables etc
BSS:                uninitialized arrays & variables
RODATA:             constant strings
COMMENT: & NOTE:    comments by the compiler / linker
STAB: & STABSTR:    debugging symbols & similar information

structure
---------
 -  ELF header
 -  program header table
 -  sections
 -  section header table
*/