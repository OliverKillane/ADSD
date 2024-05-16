#include <stdio.h>
#include <inttypes.h>

#include "hls.h"

#include "test_data.h"
#include "../test_utils.h"

void add_args(
		const int8_t image_data[PADDED_NUM_IMAGES][IMAGE_SIZE],
		hls::stream<DataWithTL<int8_t>>& xs,
		const int image_start) {
	for (int i = image_start; i < PER_STREAM + image_start; i++) {
    	for (int j = 0; j < IMAGE_SIZE; j++) {
    		xs << DataWithTL<int8_t> {
    			.data = image_data[i][j],
				.last=(j == IMAGE_SIZE - 1 && i == PER_STREAM + image_start - 1)
    		};
    	}
    }
	printf("%d data before from %d\n", xs.size(), image_start);
}

void get_rets(bool predictions[PADDED_NUM_IMAGES], hls::stream<DataWithTL<bool>>& rets, const int image_start) {
	for (int i = image_start; i < PER_STREAM + image_start; i++) {
		DataWithTL<bool> b = rets.read();
		predictions[i] = b.data;
		bool islast = i == PER_STREAM + image_start - 1;
		if (islast != b.last) {
			printf("At Image %d did not receive correct tlast signal (Expected: %d, Found %d)\n", i, islast, b.last);
		}
	}
}

void hardware_impl(const int8_t image_data[PADDED_NUM_IMAGES][IMAGE_SIZE], bool predictions[PADDED_NUM_IMAGES]) {

    hls::stream<DataWithTL<int8_t>> xs_0("image data 0");
	hls::stream<DataWithTL<int8_t>> xs_1("image data 1");
	hls::stream<DataWithTL<int8_t>> xs_2("image data 2");
	hls::stream<DataWithTL<int8_t>> xs_3("image data 3");
    hls::stream<DataWithTL<bool>> rets_0("return data 0");
	hls::stream<DataWithTL<bool>> rets_1("return data 1");
	hls::stream<DataWithTL<bool>> rets_2("return data 2");
	hls::stream<DataWithTL<bool>> rets_3("return data 3");

	add_args(image_data, xs_0, PER_STREAM * 0);
	add_args(image_data, xs_1, PER_STREAM * 1);
	add_args(image_data, xs_2, PER_STREAM * 2);
	add_args(image_data, xs_3, PER_STREAM * 3);

	nonzero(xs_0, xs_1, xs_2, xs_3, rets_0, rets_1, rets_2, rets_3);
	
	get_rets(predictions, rets_0, PER_STREAM * 0);
	get_rets(predictions, rets_1, PER_STREAM * 1);
	get_rets(predictions, rets_2, PER_STREAM * 2);
	get_rets(predictions, rets_3, PER_STREAM * 3);

}

int main() {
    bool hardware_pred[PADDED_NUM_IMAGES];
    bool software_pred[PADDED_NUM_IMAGES];
    software_impl(test_data, software_pred);
    hardware_impl(test_data, hardware_pred);
    compare_predictions(hardware_pred, software_pred);
}
