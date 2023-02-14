#include "pci.h"

RSDPDescriptor* find_rsdp() {
	const char* header = "RSD PTR ";
	for (char* iter = (char*)0x000E0000; iter <= (char*)0x000FFFFF; iter++) {
		int i;
		for (i =0; i<8; i++)
			if (iter[i] != header[i])
				break;


		if (i == 8)
			return iter;
	}
	return 0;
}
