#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

#include "xparameters.h"
#include "xnonzero.h"
#include "xaxidma.h"

#include "test_data.h"
#include "xtime_l.h"

#define NUM_IMAGES 2601
#define IMG_SIZE 784
#define PKT_SIZE 128
#define NUM_PKTS ((NUM_IMAGES * IMG_SIZE + (PKT_SIZE - 1)) / PKT_SIZE)

#define MEM_BASE_ADDR 0x01000000
#define TX_BUFFER_BASE (MEM_BASE_ADDR+0x02000000)

XNonzero mod;
XNonzero_Config* mod_config;
XAxiDma dma;
XAxiDma_Config* dma_config;

void initialise() {
	printf("Initialising\n");

	mod_config = XNonzero_LookupConfig(XPAR_NONZERO_0_DEVICE_ID);
	if (!mod_config) printf("Failed to configure mod");

	int mod_status = XNonzero_CfgInitialize(&mod, mod_config);
	if (mod_status != XST_SUCCESS) printf("Failed to configure mod %d", mod_status);

	dma_config = XAxiDma_LookupConfig(XPAR_AXIDMA_0_DEVICE_ID);
	if (!dma_config) printf("Failed to configure dma");

	int dma_status = XAxiDma_CfgInitialize(&dma, dma_config);
	if (dma_status != XST_SUCCESS) printf("Failed to configure dma %d", dma_status);

	printf("Disabling DMA interrupts\n"); // use polling mode
	XAxiDma_IntrDisable(&dma, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DEVICE_TO_DMA);
	printf("DMA 8\n");
	XAxiDma_IntrDisable(&dma, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DMA_TO_DEVICE);
}

int main() {
	printf("Test 3\n");
	bool predictions[NUM_IMAGES];

	int8_t* source_data = (int8_t*)TX_BUFFER_BASE;
	memcpy(source_data, (int8_t*)test_data, NUM_IMAGES * IMG_SIZE);

	initialise();
	printf("DMA 7\n");
	printf("\n For %d images\n| Time(ns) | Counts |\n|-|-|\n", TEST_DATA_DIM_0);
	printf("DMA 7.5\n");
	for (int i = 0; i < 10; i++) {
        XTime countsEnd, countsStart;
        XTime_GetTime(&countsStart);
		XNonzero_Start(&mod);
		Xil_DCacheFlushRange((u32) source_data , PKT_SIZE * NUM_PKTS);
		Xil_DCacheFlushRange((u32) predictions, NUM_IMAGES);
		XAxiDma_SimpleTransfer(&dma, (u32) source_data , PKT_SIZE * NUM_PKTS, XAXIDMA_DMA_TO_DEVICE);
		XAxiDma_SimpleTransfer(&dma, (u32) predictions, NUM_IMAGES, XAXIDMA_DEVICE_TO_DMA);
		while (XAxiDma_Busy(&dma, XAXIDMA_DEVICE_TO_DMA));

		Xil_DCacheInvalidateRange((u32) predictions, NUM_IMAGES);
		while (!XNonzero_IsDone(&mod));

        XTime_GetTime(&countsEnd);

        u64 counts = (countsEnd-countsStart);
        u64 timeTotal = (counts*1000000000)/(COUNTS_PER_SECOND);

        printf("|%" PRIu64 " | %" PRIu64 " |\n", timeTotal, counts);
	}
}
