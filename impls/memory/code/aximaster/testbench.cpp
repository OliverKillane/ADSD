#include <stdio.h>
#include <inttypes.h>

#include "hls.h"
#include "../test_data.h"

#define NUM_IMGS 2601

int main()
{
    bool predictions[NUM_IMGS];

    for (int i=0; i<NUM_IMGS; i++)
    {
        nonzero(test_data[i], &predictions[i]);
    }

    int nonzero_imgs = 0;
    for (int i = 0; i < NUM_IMGS; i++) {
        if (predictions[i]) {
            nonzero_imgs++;
        }
    }
    printf("%d nonzero images", nonzero_imgs);

    return 0;
}
