#include "hls.h"

void process(const int8_t x[IMG_SIZE], bool* ret) {
	bool nonzero = false;
	for (int i = 0; i < IMG_SIZE; i++) {
		nonzero = nonzero || x[i] > 0;
	}
	*ret = nonzero;
}

void nonzero(const int8_t x_0[IMG_SIZE], const int8_t x_1[IMG_SIZE], const int8_t x_2[IMG_SIZE], const int8_t x_3[IMG_SIZE], bool* ret_0, bool* ret_1, bool* ret_2, bool* ret_3) {
    #pragma HLS INTERFACE s_axilite port=return

	#pragma HLS INTERFACE m_axi port=x_0 depth=784 offset=slave bundle=image_data_0
    #pragma HLS INTERFACE m_axi port=x_1 depth=784 offset=slave bundle=image_data_1
    #pragma HLS INTERFACE m_axi port=x_2 depth=784 offset=slave bundle=image_data_2
    #pragma HLS INTERFACE m_axi port=x_3 depth=784 offset=slave bundle=image_data_3
	
    #pragma HLS INTERFACE s_axilite port=ret_0
	#pragma HLS INTERFACE s_axilite port=ret_1
	#pragma HLS INTERFACE s_axilite port=ret_2
	#pragma HLS INTERFACE s_axilite port=ret_3

	process(x_0, ret_0);
	process(x_1, ret_1);
	process(x_2, ret_2);
	process(x_3, ret_3);
}
