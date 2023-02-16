#pragma once

#include <stdint.h>
typedef uint8_t* bitset_t;

#define BITSET_SIZE(size) ((int)(size/8) + 1)

#define NUM_BIT(n,k) (n & ( 1 << k )) >> k
#define BITSET_READBIT(bitset, index) (NUM_BIT(bitset[index/8], (index%8)))

#define SET_BIT(p,n) ((p) |= (1 << (n)))
#define CLR_BIT(p,n) ((p) &= (~(1) << (n)))
#define BITSET_SETBIT(bitset, index, val) ( ((val == 0) ? CLR_BIT(bitset[index/8], index%8) : SET_BIT(bitset[index/8], index%8)) )
