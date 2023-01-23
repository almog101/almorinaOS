#pragma once
#include <stdint.h>

void out_b(uint16_t port, uint8_t value);
uint8_t in_b(uint16_t port);
void io_wait();