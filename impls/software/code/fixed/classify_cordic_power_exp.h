
#include "svs.h"
#include "alphas.h"
#include "test_data.h"
#include "cordic_bias.h"
#include "cordic_power_exp.h"
#include <cstdint>

#define IMAGE_SIZE 784 // Each image is 28x28

// Helper macros for defining fixed point types
#define FIXED_U(s, f) uint##s##_t
#define FIXED_S(s, f) int##s##_t

void cordic_step(FIXED_S(32, 12) &x, FIXED_S(32, 12) &y, FIXED_S(64, 12) &z, int iter) {
    if (z < 0) {
        FIXED_S(32, 12) new_x = x - (y >> iter);
        FIXED_S(32, 12) new_y = y - (x >> iter);
        x = new_x;
        y = new_y;
        z = z + rom[iter - 1];
    } else {
        FIXED_S(32, 12) new_x = x + (y >> iter);
        FIXED_S(32, 12) new_y = y + (x >> iter);
        x = new_x;
        y = new_y;
        z = z - rom[iter - 1];
    }
}

FIXED_S(64, 36) cordic_exp(FIXED_U(64, 2) l2Squared) {

    /* Estimate the function exp(-0.001 * l2Squared) as zero onwards */
    if (l2Squared > (3000 << 2)) {
        return 0;
    }
    
    FIXED_S(32, 12) x = initial_x;
    FIXED_S(32, 12) y = 0;
    FIXED_S(64, 12) z = -(l2Squared << 10);

    for (int iter = 1; iter <= ROM_CORDIC_ITER_END; iter++) {
        cordic_step(x, y, z, iter);
        if (iter == 4) /* double apply for iters 3k+1 with k>0 */ {
            cordic_step(x, y, z, iter);
        }
    }

    FIXED_S(64, 12) K_pre_3 = x + y;
    FIXED_S(64, 36) K = (K_pre_3 * K_pre_3 * K_pre_3);
    return K;
}

bool classify(const FIXED_S(8, 1) x[IMAGE_SIZE])
{
    FIXED_S(64, 39) sum = 0; 

    for (int i=0; i<SVS_DIM_0; i++)
    {
        FIXED_S(8, 3) alpha = alphas[i];
        FIXED_U(64, 2) l2Squared = 0;

        for (int j=0; j<IMAGE_SIZE; j++)
        {
            FIXED_S(8, 1) _sv = svs[i][j];
            FIXED_S(8, 1) _x = x[j];
            FIXED_S(32, 1) l2 = _sv - _x;
            l2Squared += ((_sv - _x) * (_sv - _x));
        }

        FIXED_S(64, 36) K = cordic_exp(l2Squared);
        sum += alpha * K;
    }

    sum += cordic_bias;
    return sum >= 0;
}