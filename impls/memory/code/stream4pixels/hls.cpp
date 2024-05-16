#include "hls.h"
#include <stdio.h>

void process_stream(hls::stream<DataWithTL<int8_t> >& xs, hls::stream<DataWithTL<bool> >& rets) {
	for (int i = 0; i < PER_STREAM; i++) {
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

void nonzero(
	hls::stream<DataWithTL<int8_t> >& xs_0, 
	hls::stream<DataWithTL<int8_t> >& xs_1, 
	hls::stream<DataWithTL<int8_t> >& xs_2, 
	hls::stream<DataWithTL<int8_t> >& xs_3, 
	hls::stream<DataWithTL<bool> >& rets_0,
	hls::stream<DataWithTL<bool> >& rets_1,
	hls::stream<DataWithTL<bool> >& rets_2,
	hls::stream<DataWithTL<bool> >& rets_3) {
#pragma HLS INTERFACE s_axilite port=return
#pragma HLS INTERFACE axis port=xs_0
#pragma HLS INTERFACE axis port=xs_1
#pragma HLS INTERFACE axis port=xs_2
#pragma HLS INTERFACE axis port=xs_3
#pragma HLS INTERFACE axis port=rets_0
#pragma HLS INTERFACE axis port=rets_1
#pragma HLS INTERFACE axis port=rets_2
#pragma HLS INTERFACE axis port=rets_3

	process_stream(xs_0, rets_0);
	process_stream(xs_1, rets_1);
	process_stream(xs_2, rets_2);
	process_stream(xs_3, rets_3);
}
