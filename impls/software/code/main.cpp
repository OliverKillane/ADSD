#include <stdio.h>
#include <inttypes.h>

#if !defined(NOXILINX)
#include "xtime_l.h"
#endif

// Select fixed point implementation
// #include "double/classify.h"
// #include "double/classify_cordic.h"
// #include "float/classify.h"
// #include "fixed/classify_table.h"
// #include "fixed/classify_cordic_power_exp.h"
#include "fixed/classify_cordic_neg_index_exp.h"

#include "ground_truth.h"

#define NUM_IMGS 2601

void summary_stats(bool predictions[NUM_IMGS]) {
    double accuracy = 0.0;
    int correct = 0;
    for (int i=0; i<NUM_IMGS; i++){
        if (predictions[i] == ground_truth[i])
            correct++;
    }
    accuracy = correct / double(NUM_IMGS);
    printf("Classification Accuracy: %f\n", accuracy);

    // confusion matrix
    double CM[2][2];
    for (int i=0; i<2; i++)
        for (int j=0; j<2; j++)
            CM[i][j]=0.0;

    for (int i=0; i<NUM_IMGS; i++){
            CM[ground_truth[i]][predictions[i]]++;
    }
    printf("Confusion Matrix (%d test points):\n", NUM_IMGS);
    printf("%f, %f\n", CM[0][0]/NUM_IMGS, CM[0][1]/NUM_IMGS);
    printf("%f, %f\n", CM[1][0]/NUM_IMGS, CM[1][1]/NUM_IMGS);
}

int main()
{
    bool predictions[NUM_IMGS];

    #if !defined(NOXILINX)
    XTime TimeEnd, TimeStart;
	XTime_GetTime(&TimeStart);
    #endif

    for (int i=0; i<NUM_IMGS; i++)
    {
        predictions[i] = classify(test_data[i]);
    }

    #if !defined(NOXILINX)
    XTime_GetTime(&TimeEnd);
 	u64 TimeHw = ((TimeEnd-TimeStart)*1000000000)/(COUNTS_PER_SECOND);
    printf("Time for %d rows in : %" PRIu64 " ns", NUM_IMGS, TimeHw);
    #endif

    summary_stats(predictions);
}
