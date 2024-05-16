#pragma once
#define PKT_SIZE 128
#define HANG_PKT_FILL 16      // 784 % 128
#define PADDED_IMAGE_SIZE 896 // 784 / 128 = 6.125, 7 * 128 = 896
#define FULL_PKTS_PER_IMG 6   // 784 // 128