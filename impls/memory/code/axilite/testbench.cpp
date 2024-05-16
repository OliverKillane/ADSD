#include <stdio.h>
#include <inttypes.h>

#include "hls.h"
#include "../test_data.h"


int main()
{
    bool predictions[NUM_IMAGES];

    for (int i=0; i<NUM_IMAGES; i++)
    {
        nonzero(test_data[i], &predictions[i]);
    }

    int nonzero_imgs = 0;
    for (int i = 0; i < NUM_IMAGES; i++) {
        if (predictions[i]) {
            nonzero_imgs++;
        }
    }
    printf("%d nonzero images", nonzero_imgs);

    return 0;
}
