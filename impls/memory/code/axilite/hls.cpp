#include "hls.h"
#include <string.h>

void nonzero(const int8_t x[IMAGE_SIZE], bool* ret) {
    #pragma HLS INTERFACE s_axilite port=return
    #pragma HLS INTERFACE s_axilite port=x
	#pragma HLS INTERFACE s_axilite port=ret

	process(x, ret);
}
