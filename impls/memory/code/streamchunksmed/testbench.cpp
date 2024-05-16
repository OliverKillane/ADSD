#include <stdio.h>
#include <inttypes.h>

#include "hls.h"
#include "../test_data.h"
#include "../test_utils.h"

#define NUM_IMGS 2601

ap_uint<PKT_SIZE_BITS> arr_ap_uint(int8_t* arr) {
	ap_uint<PKT_SIZE_BITS> conv;
	for (int i = 0; i < PKT_SIZE_BYTES; i++) {
		conv.range(7 + i * 8, i * 8) = arr[i];
	}
	return conv;
}

void ap_uint_arr(ap_uint<PKT_SIZE_BITS> ap, int8_t arr[PKT_SIZE_BYTES]) {
	for (int i = 0; i < PKT_SIZE_BYTES; i++) {
		arr[i] = ap.range(7 + i * 8, i * 8);
	}
}

void test_conversions() {
	int8_t x[PKT_SIZE_BYTES] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
	int8_t x_conv[PKT_SIZE_BYTES];

	ap_uint<PKT_SIZE_BITS> ap_conv = arr_ap_uint(x);

	ap_uint_arr(ap_conv, x_conv);

	bool mismatch = false;
	for (int i = 0; i < PKT_SIZE_BYTES; i++) {
		if (x[i] != x_conv[i]) {
			printf("Mismatch: %d -> ap_uint -> %d\n", x[i], x_conv[i]);
			mismatch = true;
		}
	}
	if (!mismatch) {
		printf("No mismatch in conversions\n");
	}
}

void hardware_impl(const int8_t image_data[NUM_IMAGES][IMAGE_SIZE], bool predictions[NUM_IMAGES]) {
	hls::stream<PixelPacket> xs("image data");
	hls::stream<BoolPacket> rets("return data");

	for (int image_index = 0; image_index < NUM_IMAGES; image_index++) {
		for (int offset = 0; offset < IMAGE_SIZE; offset+= PKT_SIZE_BYTES) {
			PixelPacket p;
			p.data = arr_ap_uint((int8_t*)&image_data[image_index][offset]);
			p.last = (image_index == image_index - 1) && (offset == IMAGE_SIZE - 1);
			xs << p;
		}
	}
	printf("%d data before\n", xs.size());
	nonzero(xs, rets);
	printf("%d data after\n", xs.size());
	for (int i = 0; i < NUM_IMAGES; i++) {
		BoolPacket b = rets.read();
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

    test_conversions();
}
