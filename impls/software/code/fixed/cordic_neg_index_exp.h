#pragma once
#include <stdint.h>
#define ROM_FRAC_BITS 10
#define ROM_FRAC_DIV 1024
#include <stdint.h>
#define INITIAL_X_FRAC_BITS 12
#define INITIAL_X_FRAC_DIV 4096
// generated with bake factor 1000.0 for range -4 to 7
#define ROM_CORDIC_ITER_END 7
#define ROM_CORDIC_ITER_START -4
// The thetamax for this implementation is 9593.104193726702
#define ROM_DIM_0 12
typedef int64_t /* frac=10 */ rom_t;
const static rom_t rom[12] = {
2480223,
2121284,
1758201,
1386521,
996305,
562489,
261542,
128672,
64083,
32010,
16001,
8000,
}
;
typedef int64_t /* frac=12 */ initial_x_t;
const static initial_x_t initial_x = 1016234
;
