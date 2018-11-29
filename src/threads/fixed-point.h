#ifndef FIXED_POINT_H
#define FIXED_POINT_H

#include <stdint.h>

typedef int32_t fixed32_t;

fixed32_t fraction = 2 << 14;
fixed32_t half_fraction = 2 << 13;

fixed32_t int2fixed(int x);
int fixed2int(fixed32_t x);
int fixed2int_trunc(fixed32_t x);

fixed32_t fixed_add(fixed32_t x, fixed32_t y);
fixed32_t fixed_sub(fixed32_t x, fixed32_t y);
fixed32_t fixed_mul(fixed32_t x, fixed32_t y);
fixed32_t fixed_div(fixed32_t x, fixed32_t y);

fixed32_t fixed_add_int(fixed32_t x, int y);
fixed32_t fixed_sub_int(fixed32_t x, int y);
fixed32_t fixed_mul_int(fixed32_t x, int y);
fixed32_t fixed_div_int(fixed32_t x, int y);

#endif
