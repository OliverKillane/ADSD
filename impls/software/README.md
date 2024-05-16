## Software Implementations

### Why?
To get a baseline to assess if it is worth computing the svm on the fabric at all.
- Get the minimum speed, from just running on a single arm core. 

### Results

Run on a single arm core on the zedboard.

| Execution | Types  | Technique | Accuracy    | Time for all   | Per Image   |
|-----------|--------|-----------|-------------|----------------|-------------|
| Software  | double | cordic    | $99.7309\%$ | 12689056924 ns | 4.8785 ms   |
| Software  | double | naive     | $99.7309\%$ |  7746311985 ns | 2.9782 ms   |
| Software  | float  | naive     | $99.7309\%$ |  5101110062 ns | 1.9612 ms   |
| Software  | fixed  | cordic    | $99.7309\%$ |  4546716321 ns | 1.7481 ms   |
| Software  | fixed  | tabulated | $99.7309\%$ |  2908732502 ns | 1.1183 ms   |
