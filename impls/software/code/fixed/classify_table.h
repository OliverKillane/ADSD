#include <math.h>

#include "svs.h"
#include "tab_bias.h"
#include "alphas.h"
#include "test_data.h"

#include "exponent_table.h"

#define IMG_SIZE 784 // Each image is 28x28


#include <stdio.h>

#define FIXED_U(s, f) uint##s##_t
#define FIXED_S(s, f) int##s##_t

bool classify(const test_data_t x[IMG_SIZE])
{
    FIXED_S(64, 8) sum = tab_bias;
    for (int i=0; i<SVS_DIM_0; i++)
    {
        FIXED_S(8, 3) alpha = alphas[i];
    
        FIXED_S(32, 2) l2Squared = 0;
        for (int j=0; j<IMG_SIZE; j++)
        {
            FIXED_S(8, 1) _sv = svs[i][j];
            FIXED_S(8, 1) _x = x[j];
            l2Squared += ((_sv - _x) * (_sv - _x));
        }

        if (l2Squared < (3000 << 2)) {
            /* Ignore larger values */
            FIXED_S(64, 21) K = (FIXED_S(64, 21))index_exponent_table(l2Squared);
            sum += (alpha * K) >> 16;
        }
    }

    return sum >= 0;
}