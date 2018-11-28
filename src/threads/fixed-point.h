#ifndef FIXED_POINT_H
#define FIXED_POINT_H

#include <stdint.h>

typedef int32_t fixed32_t;

fixed32_t fraction = 2 << 14;
fixed32_t half_fraction = 2 << 13;

fixed32_t int2fixed(int x) {return x * fraction;}
int fixed2int(fixed32_t x)
{
    if(x < 0) return (x - half_fraction) / fraction;
    else (x + half_fraction) / fraction;
}
int fixed2int_trunc(fixed32_t x) {return x / fraction;}

fixed32_t fixed_add(fixed32_t x, fixed32_t y) {return x + y;}
fixed32_t fixed_sub(fixed32_t x, fixed32_t y) {return x - y;}
fixed32_t fixed_mul(fixed32_t x, fixed32_t y) {return (((int64_t)x) * y) / fraction;}
fixed32_t fixed_div(fixed32_t x, fixed32_t y) {return (((int64_t)x) * fraction) / y;}

fixed32_t fixed_add_int(fixed32_t x, int y) {return x + y * fraction;}
fixed32_t fixed_sub_int(fixed32_t x, int y) {return x - y * fraction;}
fixed32_t fixed_mul_int(fixed32_t x, int y) {return x * y;}
fixed32_t fixed_div_int(fixed32_t x, int y) {return x / y;}

#endif
