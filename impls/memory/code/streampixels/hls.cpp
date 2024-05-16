#include "hls.h"
#include <stdio.h>

void nonzero(hls::stream<DataWithTL<int8_t> >& xs, hls::stream<DataWithTL<bool> >& rets) {
	#pragma HLS INTERFACE s_axilite port=return
	#pragma HLS INTERFACE axis port=xs
	#pragma HLS INTERFACE axis port=rets

	for (int i = 0; i < NUM_IMAGES; i++) {
		bool nonzero = false;
		DataWithTL<int8_t> v;
		for (int j = 0; j < IMAGE_SIZE; j++) {
			v = xs.read();
			nonzero = nonzero || v.data > 2;
		}
		DataWithTL<bool> r;
		r.data = nonzero;
		r.last = v.last;
		rets.write(r);
	}
}
