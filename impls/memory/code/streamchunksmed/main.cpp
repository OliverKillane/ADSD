#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

#include "xparameters.h"
#include "xnonzero.h"
#include "xaxidma.h"

#include "test_data.h"
#include "xtime_l.h"

#include "common.h"

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
	XAxiDma_IntrDisable(&dma, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DMA_TO_DEVICE);
}

int main() {
	printf("Test 4\n");
	bool predictions[NUM_IMAGES];

	int8_t* source_data = (int8_t*)test_data;
	initialise();

	printf("\n For %d images\n| Time(ns) | Counts |\n|-|-|\n", NUM_IMAGES);
	for (int i = 0; i < 10; i++) {
        XTime countsEnd, countsStart;
        XTime_GetTime(&countsStart);
		XNonzero_Start(&mod);
		Xil_DCacheFlushRange((u32) source_data , NUM_IMAGES * IMAGE_SIZE);
		Xil_DCacheFlushRange((u32) predictions, NUM_IMAGES);

		XAxiDma_SimpleTransfer(&dma, (u32) source_data , NUM_IMAGES * IMAGE_SIZE, XAXIDMA_DMA_TO_DEVICE);
		XAxiDma_SimpleTransfer(&dma, (u32) predictions, NUM_IMAGES, XAXIDMA_DEVICE_TO_DMA);

		while (XAxiDma_Busy(&dma, XAXIDMA_DEVICE_TO_DMA));

		Xil_DCacheInvalidateRange((u32) predictions, NUM_IMAGES);
		while (!XNonzero_IsDone(&mod));

        XTime_GetTime(&countsEnd);

        u64 counts = (countsEnd-countsStart);
        u64 timeTotal = (counts*1000000000)/(COUNTS_PER_SECOND);

        printf("|%" PRIu64 " | %" PRIu64 " |\n", timeTotal, counts);
	}

	// test last results
	int zero_cnt = 0;
	for (int i = 0 ; i < NUM_IMAGES; i++) {
		if (predictions[i]) {
			zero_cnt++;
		}
	}
	printf("zero_cnt: %d\n", zero_cnt);
}
