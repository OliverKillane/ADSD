#include <stdint.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>

#define IMG_SIZE 784 // Each image is 28x28
#define NUM_IMAGES 2601
#define PKT_SIZE 128
#define NUM_PKTS ((NUM_IMAGES * IMG_SIZE + (PKT_SIZE - 1)) / PKT_SIZE)

struct BoolPacket {
	bool data;
	ap_uint<1> last;
};

struct PixelPacket {
	int8_t data[PKT_SIZE];
};

void nonzero(hls::stream<PixelPacket>& xs, hls::stream<BoolPacket>& rets);
