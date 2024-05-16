#include <stdint.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>
#include "../common.h"

template<typename T>
struct DataWithTL {
	T data;
	ap_uint<1> last;
};

void nonzero(hls::stream<DataWithTL<int8_t> >& xs, hls::stream<DataWithTL<bool> >& rets);
