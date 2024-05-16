#pragma once
#include <stdint.h>
#define CORDIC_BIAS_FRAC_BITS 39
#define CORDIC_BIAS_FRAC_DIV 549755813888
typedef int64_t cordic_bias_t;
const static cordic_bias_t cordic_bias = -99413343066
;
