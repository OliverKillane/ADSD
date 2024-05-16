#include <stdio.h>
#include <inttypes.h>
#include "xnonzero.h"

// remove const qualifier
#include "test_data.h"
#include "xtime_l.h"
#include "xparameters.h"

int main() {
	bool predictions[TEST_DATA_DIM_0];
    XNonzero mod;
    XNonzero_Initialize(&mod, XPAR_NONZERO_0_DEVICE_ID);

    printf("\n For %d images\n| Time(ns) | Counts |\n|-|-|\n", TEST_DATA_DIM_0);
    for (int i = 0; i < 10; i++) {
        XTime countsEnd, countsStart;

        XTime_GetTime(&countsStart);

        for (int i = 0; i < TEST_DATA_DIM_0; i++) {
            XNonzero_Set_x(&mod, (u32) test_data[i]);
        	XNonzero_Start(&mod);

        	bool res;
        	do {
        		res = XNonzero_Get_ret(&mod);
        	} while (!XNonzero_IsReady(&mod));
        	predictions[i] = res;
        }
        XTime_GetTime(&countsEnd);

        u64 counts = (countsEnd-countsStart);
        u64 timeTotal = (counts*1000000000)/(COUNTS_PER_SECOND);

        printf("|%" PRIu64 " | %" PRIu64 " |\n", timeTotal, counts);
    }
}
