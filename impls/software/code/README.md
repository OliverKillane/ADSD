## Software Implementations

### Setup
See each individual directory

For ground truth
```bash
qua --csv-path=../../../models/0_1/weights/ground_truth.csv --bools --signed=false --out-dir=. --name=ground_truth
```

### Testing
Uncomment your chosen implementation include.

To compile and run with a non-vivado toolchain compiler.
```bash
g++ main.cpp -DNOXILINX -o test.out
./test.out
```