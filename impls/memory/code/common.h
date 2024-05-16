#pragma once
#define NUM_IMAGES 2601
#define IMAGE_SIZE 784 // Each image is 28x28

static void process(const int8_t x[IMAGE_SIZE], bool* ret) {
	bool nonzero = false;
	for (int i = 0; i < IMAGE_SIZE; i++) {
		const int8_t pixel = x[i];
        nonzero = nonzero || pixel > 0;
	}
	*ret = nonzero;
}
