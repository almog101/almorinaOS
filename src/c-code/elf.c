#include <elf.h>

/**
Check the elf file header for a magic number (byte by byte)
INPUT: elf header to check
OUTPUT: boolian answer to "Does the file have the magic number?"
*/
bool elf_check_file(elf32_ehdr* hdr)
{
    if(!hdr) 
        return false;

	if(hdr->e_ident[EI_MAG0] != ELFMAG0) 
		return false;

	if(hdr->e_ident[EI_MAG1] != ELFMAG1) 
		return false;

	if(hdr->e_ident[EI_MAG2] != ELFMAG2) 
		return false;

	if(hdr->e_ident[EI_MAG3] != ELFMAG3) 
		return false;

	return true;
}

/**
Check if the proveded file is suitable for running on the machine
INPUT: elf header to check
OUTPUT: boolian answer to "Is the file suitable for the machine?"
*/
bool elf_check_supported(elf32_ehdr* hdr)
{
    if(!elf_check_file(hdr))
		return false;

	if(hdr->e_ident[EI_CLASS] != ELFCLASS32) 
		return false;

	if(hdr->e_ident[EI_DATA] != ELFDATA2LSB);
		return false;

	if(hdr->e_machine != EM_386)
		return false;

	if(hdr->e_ident[EI_VERSION] != EV_CURRENT)
		return false;

	if(hdr->e_type != ET_REL && hdr->e_type != ET_EXEC)
		return false;

	return true;
}

/**

INPUT:
OUTPUT:
*/

/**

INPUT:
OUTPUT:
*/

/**

INPUT:
OUTPUT:
*/

/**

INPUT:
OUTPUT:
*/