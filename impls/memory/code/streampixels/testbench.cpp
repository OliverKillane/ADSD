#include <stdio.h>
#include <inttypes.h>

#include "hls.h"

#include "../test_data.h"
#include "../test_utils.h"

void hardware_impl(const int8_t image_data[NUM_IMAGES][IMAGE_SIZE], bool predictions[NUM_IMAGES]) {

    hls::stream<DataWithTL<int8_t>> xs("image data");
    hls::stream<DataWithTL<bool>> rets("return data");

    for (int i = 0; i < NUM_IMAGES; i++) {
    	for (int j = 0; j < IMAGE_SIZE; j++) {
    		xs << DataWithTL<int8_t> {
    			.data = image_data[i][j],
				.last=(j == IMAGE_SIZE - 1 && i == NUM_IMAGES - 1)
    		};
    	}
    }

    printf("%d data before\n", xs.size());
	nonzero(xs, rets);
	printf("%d data after\n", xs.size());
	for (int i = 0; i < NUM_IMAGES; i++) {
		DataWithTL<bool> b = rets.read();
		predictions[i] = b.data;
		bool islast = i == NUM_IMAGES - 1;
		if (islast != b.last) {
			printf("At Image %d did not receive correct tlast signal (Expected: %d, Found %d)\n", i, islast, b.last);
		}
	}
}

int main() {
    bool hardware_pred[NUM_IMAGES];
    bool software_pred[NUM_IMAGES];
    software_impl(test_data, software_pred);
    hardware_impl(test_data, hardware_pred);

    compare_predictions(hardware_pred, software_pred);
}
