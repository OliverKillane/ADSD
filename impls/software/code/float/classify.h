#include <math.h>

#include "svs.h"
#include "bias.h"
#include "alphas.h"
#include "test_data.h"

#define IMG_SIZE 784 // Each image is 28x28
#define NUM_IMGS 2601

bool classify(const float x[IMG_SIZE])
{
    float sum = 0.0;
    for (int i=0; i<SVS_DIM_0; i++)
    {
        float alpha = alphas[i];

        float l2Squared = 0.0;
        for (int j=0; j<IMG_SIZE; j++)
        {
            float _sv = svs[i][j];
            float _x = x[j];
            l2Squared += (_sv - _x) * (_sv - _x);
        }
        float K = exp(-0.001 * l2Squared);

        sum += alpha * K;
    }
    sum = sum + bias;
    return sum >= 0;
}