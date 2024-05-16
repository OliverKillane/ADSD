#pragma once
#include <stdint.h>
#define TAB_BIAS_FRAC_BITS 8
#define TAB_BIAS_FRAC_DIV 256
typedef int64_t /* frac=8 */ tab_bias_t;
const static tab_bias_t tab_bias = -46
;
