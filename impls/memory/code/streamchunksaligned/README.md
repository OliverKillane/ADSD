## Alignment
In order to fit all into a very wide packet, we pad the images to add ignored space.

This is contained in the [`pkt_info.h`](./pkt_info.h) file.
As a result the normal output from `qua` needs to be ammended for [`test_data.h`](./test_data.h).

```cpp
#pragma once
#include "pkt_info.h"

#include <stdint.h>
#define TEST_DATA_FRAC_BITS 1
#define TEST_DATA_FRAC_DIV 2
#define TEST_DATA_DIM_0 2601
#define TEST_DATA_DIM_1 (PADDED_IMAGE_SIZE)
typedef int8_t test_data_t;
const static test_data_t test_data[2601][PADDED_IMAGE_SIZE] = {
    // ...
};
```