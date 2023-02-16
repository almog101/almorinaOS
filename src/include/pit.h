#ifndef PIT_H
#define PIT_H

#include <stdint.h>

void Sleepd(double seconds);
void Sleep(uint64_t milliseconds);

void PIT_set_divisor(uint16_t divisor);
uint64_t PIT_get_frequency();
void PIT_set_frequency(uint64_t frequency);
void PIT_tick();

void PIT_set_counter(double amount_seconds);
double PIT_get_counter();

#endif
