#include "hls.h"

bool pkt_nonzero(const PixelPacket pkt) {
    bool nonzero;
    for (int i = 0; i < PKT_SIZE; i++) {
        nonzero = nonzero || pkt.data[i] > 0;
    }
    return nonzero;
}

bool pkt_end_nonzero(const PixelPacket pkt) {
    bool nonzero;
    for (int i = 0; i < HANG_PKT_FILL; i++) {
        nonzero = nonzero || pkt.data[i] > 0;
    }
    return nonzero;
}


void nonzero(hls::stream<PixelPacket>& xs, hls::stream<BoolPacket>& rets) {
	#pragma HLS INTERFACE s_axilite port=return
	#pragma HLS DATA_PACK variable=xs
	#pragma HLS INTERFACE axis port=xs
	#pragma HLS INTERFACE axis port=rets

    for (int image_index = 0; image_index < NUM_IMAGES; image_index++) {
    	bool pkt_1_nonzero = pkt_nonzero(xs.read());
        bool pkt_2_nonzero = pkt_nonzero(xs.read());
        bool pkt_3_nonzero = pkt_nonzero(xs.read());
        bool pkt_4_nonzero = pkt_nonzero(xs.read());
        bool pkt_5_nonzero = pkt_nonzero(xs.read());
        bool pkt_6_nonzero = pkt_nonzero(xs.read());
        bool pkt_end_7_nonzero = pkt_end_nonzero(xs.read());

        bool res = pkt_1_nonzero || pkt_2_nonzero || pkt_3_nonzero || pkt_4_nonzero || pkt_5_nonzero || pkt_6_nonzero || pkt_end_7_nonzero;

        BoolPacket r;
        r.data = res;
        r.last = image_index == NUM_IMAGES - 1;
        rets.write(r);
    }
}
