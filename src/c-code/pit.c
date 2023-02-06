#include "stdint.h"
#include "ports.h"
#include "io.h"
#include <stdio.h>

static double time_since_boot = 0;
static const uint64_t base_frequency = 1193182;

uint16_t PIT_divisor = 65535;

void Sleepd(double seconds){
	double start_time = time_since_boot;
	while (time_since_boot < start_time + seconds){
		asm("hlt");
	}
}

void Sleep(uint64_t milliseconds){
	Sleepd((double)milliseconds / 1000);
}

void PIT_set_divisor(uint16_t divisor) {
	if (divisor < 100) divisor = 100;
	PIT_divisor = divisor;
	outb(0x40, (uint8_t)(divisor & 0x00ff));
	io_wait();
	outb(0x40, (uint8_t)((divisor & 0xff00) >> 8));
}

uint64_t PIT_get_frequency() {
	return base_frequency / PIT_divisor;
}

void PIT_set_frequency(uint64_t frequency) {
	PIT_set_divisor(base_frequency / frequency);
}

void PIT_tick() {
	time_since_boot += 1 / (double)PIT_get_frequency();
}
