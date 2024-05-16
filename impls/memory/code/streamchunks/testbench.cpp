#include <stdio.h>
#include <inttypes.h>

#include "hls.h"
#include "../test_data.h"

#define NUM_IMGS 2601

int main()
{
    bool predictions[NUM_IMGS];
    hls::stream<PixelPacket> xs("image data");
    hls::stream<BoolPacket> rets("return data");

    for (int pkt_index = 0; pkt_index < NUM_PKTS; pkt_index++) {
        PixelPacket pkt;
        for (int i = 0; i < PKT_SIZE; i++) {

        	int img_index = (pkt_index * PKT_SIZE + i) / IMG_SIZE;
        	int pixel_index = (pkt_index * PKT_SIZE + i) % IMG_SIZE;

        	if (img_index < TEST_DATA_DIM_0 && pixel_index < TEST_DATA_DIM_1) {
        		pkt.data[i] = test_data[img_index][pixel_index];
        	} else {
        		pkt.data[i] = 0;
        	}
        }
        xs << pkt;
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

    return 0;
}
