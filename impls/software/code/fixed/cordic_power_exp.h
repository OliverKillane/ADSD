#pragma once
#include <stdint.h>
#define ROM_FRAC_BITS 12
#define ROM_FRAC_DIV 4096
#include <stdint.h>
#define INITIAL_X_FRAC_BITS 12
#define INITIAL_X_FRAC_DIV 4096
// generated with bake factor 3000.0 for range 1 to 7
#define ROM_CORDIC_ITER_END 7
#define ROM_CORDIC_ITER_START 1
#define ROM_DIM_0 7
typedef int64_t /* frac=12 */ rom_t;
const static rom_t rom[7] = {
6749873,
3138512,
1544075,
769002,
384125,
192015,
96001,
}
;
typedef int64_t /* frac=12 */ initial_x_t;
const static initial_x_t initial_x = 4945
;
