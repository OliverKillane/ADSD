// NOTE:
// Include after the test_data is included and predictions defined.

#pragma once
#include <stdio.h>

void software_impl(const test_data_t image_data[NUM_IMAGES][IMAGE_SIZE],
                   bool predictions[NUM_IMAGES]) {
  for (int i = 0; i < NUM_IMAGES; i++) {
    bool nonzero = false;
    for (int j = 0; j < IMAGE_SIZE; j++) {
      nonzero = nonzero || (image_data[i][j] > 2);
    }
    predictions[i] = nonzero;
  }
}

void compare_predictions(const bool hardware_pred[NUM_IMAGES],
                         const bool software_pred[NUM_IMAGES]) {
  int sw_count = 0;
  int hw_count = 0;
  for (int i = 0; i < NUM_IMAGES; i++) {
    if (hardware_pred[i])
      hw_count++;
    if (software_pred[i])
      sw_count++;
    if (hardware_pred[i] != software_pred[i]) {
      printf("Mismatch at image %d\n  Software:%d\n  Hardware:%d\n", i,
             software_pred[i], hardware_pred[i]);
    }
  }

  if (sw_count == hw_count) {
    printf("Software and Hardware Predictions Match at %d!\n", sw_count);
  } else {
    printf("Mismatch between results of software (%d) and hardware (%d)\n",
           sw_count, hw_count);
  }
}
