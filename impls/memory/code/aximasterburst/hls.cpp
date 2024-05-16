#include "hls.h"
#include <string.h>

void nonzero(const int8_t x[IMAGE_SIZE], bool* ret) {
    // we keep output and return on axilite, only the array is transfered as a burst
    #pragma HLS INTERFACE s_axilite port=return
    // For some reason the IMAGE_SIZE macro is not working inside the HLS pragma as in:
	// https://docs.xilinx.com/r/2020.2-English/ug1399-vitis-hls/Using-define-with-Pragma-Directives
    // Included in a bundle to show name in vivado (technically unnecessary as we only have one parameter)
	#pragma HLS INTERFACE m_axi port=x depth=784 offset=slave bundle=image_data
	#pragma HLS INTERFACE s_axilite port=ret

    // burst copy the image
    int8_t d[IMAGE_SIZE];
	memcpy(d, x, IMAGE_SIZE);

    process(d, ret);
}
