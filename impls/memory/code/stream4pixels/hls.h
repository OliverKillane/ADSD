#include <stdint.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include "../common.h"

#define PER_STREAM 651
#define PADDED_NUM_IMAGES 2604

template<typename T>
struct DataWithTL {
	T data;
	ap_uint<1> last;
};

void nonzero(
	hls::stream<DataWithTL<int8_t> >& xs_0, 
	hls::stream<DataWithTL<int8_t> >& xs_1,
	hls::stream<DataWithTL<int8_t> >& xs_2, 
	hls::stream<DataWithTL<int8_t> >& xs_3, 
	hls::stream<DataWithTL<bool> >& rets_0,
	hls::stream<DataWithTL<bool> >& rets_1,
	hls::stream<DataWithTL<bool> >& rets_2,
	hls::stream<DataWithTL<bool> >& rets_3);
