#include "hls.h"

bool pkt_nonzero(const PixelPacket pkt) {
	ap_int<8> state[16];
	for (int i = 0; i < PKT_SIZE_BYTES; i++) {
	#pragma HLS UNROLL
		state[i] = pkt.data.range(7 + i * 8, i * 8);
	}

	bool nonzero = false;
    for (int i = 0; i < PKT_SIZE_BYTES; i++) {
        nonzero = state[i] > 2 || nonzero;
    }
    return nonzero;
}

void nonzero(hls::stream<PixelPacket>& xs, hls::stream<BoolPacket>& rets) {
	#pragma HLS INTERFACE s_axilite port=return
	#pragma HLS INTERFACE axis port=xs
	#pragma HLS INTERFACE axis port=rets

    for (int image_index = 0; image_index < NUM_IMAGES; image_index++) {
        bool res = false;
    	for (int pkt_index = 0; pkt_index < PKT_PER_IMAGE; pkt_index++) {
            bool curr_res = pkt_nonzero(xs.read());
            res = res || curr_res;
        }

        BoolPacket r;
        r.data = res;
        r.last = image_index == NUM_IMAGES - 1;
        rets.write(r);
    }
}
