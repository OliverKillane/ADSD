#include <stdio.h>
#include <inttypes.h>

#include "hls.h"
#include "test_data.h"

#define NUM_IMGS 2601

int main()
{
    bool predictions[NUM_IMGS];
    hls::stream<PixelPacket> xs("image data");
    hls::stream<BoolPacket> rets("return data");

    for (int image_index = 0; image_index < NUM_IMGS; image_index++) {
        for (int offset = 0; offset < PADDED_IMAGE_SIZE; offset+= PKT_SIZE) {
            PixelPacket p;
            for (int inner = 0; inner < PKT_SIZE; inner++) {
                p.data[inner] = test_data[image_index][offset + inner];
            }
            xs << p;
        }
    }

    printf("%d data\n", xs.size());

    nonzero(xs, rets);

    int nonzero_imgs = 0;
    for (int i = 0; i < NUM_IMGS; i++) {
    	if (rets.read().data) {
            nonzero_imgs++;
        }
    }
    printf("%d nonzero images\n", nonzero_imgs);
    printf("%d data\n", xs.size());
}
