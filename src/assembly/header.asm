; multiboot specification is a set of conventions for how a kernel should get loaded into memory
; since we use GRUB, the bootloader requers a header with certain information
; this is the header [duh]

section .multiboot_header

header_start:
	dd 0xe85250d6		; magic number that is meant for multiboot2
	dd 0 				; number to specify  what mode to enter
						; 0 = protected mode [i386]
	dd header_end - header_start
						; header length
	dd 0x100000000 - (0xe85250d6 + 0 + (header_end - header_start))
						; checksum for GRUB to reasure that evrything is accurate
	
	; the following are end tags that are used to:
	;  -	to do something extra before handling the control to our kernel
	;  -	give out kernel some extra information

	dw 0				; type
	dw 0				; flags
	dd 8				; size of tag [including header fields]
header_end:
