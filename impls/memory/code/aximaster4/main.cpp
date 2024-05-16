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

        for (int i = 0; i < 2600; i += 4) {
//        	printf("%d\n", i);
        	XNonzero_Set_x_0(&mod, (u32) test_data[i]);
        	XNonzero_Set_x_1(&mod, (u32) test_data[i+1]);
        	XNonzero_Set_x_2(&mod, (u32) test_data[i+2]);
        	XNonzero_Set_x_3(&mod, (u32) test_data[i+3]);

        	XNonzero_Start(&mod);
        	bool res_0, res_1, res_2, res_3;
        	do {
        		res_0 = (bool)XNonzero_Get_ret_0(&mod);
                res_1 = (bool)XNonzero_Get_ret_1(&mod);
                res_2 = (bool)XNonzero_Get_ret_2(&mod);
                res_3 = (bool)XNonzero_Get_ret_3(&mod);
        	}
        	while (!XNonzero_IsReady(&mod));

            predictions[i] = res_0;
            predictions[i+1] = res_1;
            predictions[i+2] = res_2;
            predictions[i+3] = res_3;
        }

        // Do final images
         XNonzero_Set_x_0(&mod, (u32) test_data[2600]);
         XNonzero_Set_x_1(&mod, (u32) test_data[2601]);
         XNonzero_Start(&mod);
         while (!XNonzero_IsReady(&mod));
         predictions[2600] = (bool)XNonzero_Get_ret_0(&mod);
         predictions[2601] = (bool)XNonzero_Get_ret_0(&mod);

        XTime_GetTime(&countsEnd);

        u64 counts = (countsEnd-countsStart);
        u64 timeTotal = (counts*1000000000)/(COUNTS_PER_SECOND);

        printf("|%" PRIu64 " | %" PRIu64 " |\n", timeTotal, counts);
    }
}
