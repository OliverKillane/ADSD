#include "hls.h"

void nonzero(hls::stream<PixelPacket>& xs, hls::stream<BoolPacket>& rets) {
	#pragma HLS INTERFACE s_axilite port=return
	#pragma HLS DATA_PACK variable=xs
	#pragma HLS INTERFACE axis port=xs
	#pragma HLS INTERFACE axis port=rets

    bool nonzero = false;
    int pixel_index = 0;

    for (int pkt_index = 0; pkt_index < NUM_PKTS; pkt_index++) {
        PixelPacket v = xs.read();
        for (int data_index = 0; data_index < PKT_SIZE; data_index++) {
            nonzero = nonzero || v.data[data_index] > 0;
            pixel_index++;
            if (pixel_index == IMG_SIZE) {
            	pixel_index = 0;
            	BoolPacket r;
                r.data = nonzero;
                r.last = pkt_index == NUM_PKTS - 1;
                rets.write(r);
                nonzero = false;
            }
        }
    }
}
