
#include "svs.h"
#include "bias.h"
#include "alphas.h"
#include "test_data.h"

#include <math.h>
#include "cordic_exp.h"

#define IMG_SIZE 784 // Each image is 28x28
#define NUM_IMGS 2601

void cordic_step(double& x, double& y, double& z, int iter) {
    double di;
    if (z < 0) {
        di = -1;
    } else {
        di = 1;
    }

    double new_x = x + di * y * pow(2, -iter);
    double new_y = y + di * x * pow(2, -iter);
    x = new_x;
    y = new_y;

    z = z - di * rom[iter];
}

double cordic_exp(double l2Squared) {

    if (l2Squared > 3000.0) {
		return 0;
	}

    double curr_x = initial_x;
    double curr_y = 0.0;
    double curr_z = -l2Squared;


    for (int iter = 1; iter <= ROM_CORDIC_ITERS; iter++) {
        cordic_step(curr_x, curr_y, curr_z, iter);
        if (iter == 4) {
            cordic_step(curr_x, curr_y, curr_z, iter);
        }
    }

    return curr_x + curr_y;
}

bool classify(const double x[IMG_SIZE])
{
    double sum = 0.0;
    for (int i=0; i<SVS_DIM_0; i++)
    {
        double alpha = alphas[i];

        double l2Squared = 0.0;
        for (int j=0; j<IMG_SIZE; j++)
        {
            double _sv = svs[i][j];
            double _x = x[j];
            l2Squared += (_sv - _x) * (_sv - _x);
        }
        double K = pow(cordic_exp(l2Squared), 3);

        sum += alpha * K;
    }
    sum = sum + bias;
    return sum >= 0;
}
