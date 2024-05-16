## Comparison of different data transfer methods

```bash
qua --csv-path=../../../models/0_1/weights/ground_truth.csv --bools --signed=false         --out-dir=. --name=ground_truth
qua --csv-path=../../../models/0_1/weights/test_data.csv    --fixed-size=8 --fixed-frac=1  --out-dir=. --name=test_data
```