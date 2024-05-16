#include <stdio.h>
#include <inttypes.h>

#include "hls.h"
#include "../test_data.h"

#define NUM_IMGS 2601

int main()
{
    bool predictions[NUM_IMGS];

    for (int i=0; i<2600; i+=4)
    {
        nonzero(test_data[i], test_data[i+1], test_data[i+2], test_data[i+3], &predictions[i],&predictions[i+1],&predictions[i+2],&predictions[i+3]);
    }

    bool dummy;
    nonzero(test_data[2600], test_data[2601], test_data[0], test_data[0], &predictions[2600],&predictions[2601],&dummy,&dummy);

    int nonzero_imgs = 0;
    for (int i = 0; i < NUM_IMGS; i++) {
        if (predictions[i]) {
            nonzero_imgs++;
        }
    }
    printf("%d nonzero images", nonzero_imgs);

    return 0;
}
