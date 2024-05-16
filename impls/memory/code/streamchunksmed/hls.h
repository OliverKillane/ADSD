#include <stdint.h>
#include <hls_stream.h>
#include <ap_axi_sdata.h>

#include "../common.h"

#define PKT_PER_IMAGE 49
#define PKT_SIZE_BYTES 16
#define PKT_SIZE_BITS 128

struct BoolPacket {
	bool data;
	ap_uint<1> last;
};

struct PixelPacket {
	ap_uint<PKT_SIZE_BITS> data;
    ap_uint<1> last;
};

void nonzero(hls::stream<PixelPacket>& xs, hls::stream<BoolPacket>& rets);
