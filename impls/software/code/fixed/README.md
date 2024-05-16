## Fixed Point
We include the svm supports as `int8_t` arrays.

```bash
qua --csv-path=../../../../models/0_1/weights/svs.csv       --fixed-size=8 --fixed-frac=1  --out-dir=. --name=svs
qua --csv-path=../../../../models/0_1/weights/alphas.csv    --fixed-size=8 --fixed-frac=3  --out-dir=. --name=alphas
qua --csv-path=../../../../models/0_1/weights/test_data.csv --fixed-size=8 --fixed-frac=1  --out-dir=. --name=test_data
```

For the [table-based implementation](./classify_table.h) we need to generate a table of the values for $K = exp(-0.001 \times x) \ where \ x = \left\| x_i - x \right\|^2 $
- The range is based on the min/max of the L2 values.
- The number of fractional bits was determined using the built in fractional bit optimisation
```bash
tab --range-start=32.75 --range-end=23759.25 --index-frac=2 --expr="math.exp(-0.001 * x)" --name=exponent_table --out-dir=. --fixed-size=32 --fixed-frac=8
```

The bias is a single integer, and is added to the sum at the end, to keep it's fraction aligned properly for the [table implementation](./classify_table.h) we use the same offset as `K` which uses `ALPHAS_FRAC_BITS + EXP_FRAC_BITS = 3 + (1 + 1)` fractional bits. 
- Going for a higher accuracy than the sum is redundant.
```bash
qua --csv-path=../../../../models/0_1/weights/bias.csv --fixed-size=64 --fixed-frac=8  --out-dir=. --name=tab_bias
```

For cordic however, we want the bias to match the output size, so we have another bias header:
```bash
qua --csv-path=../../../../models/0_1/weights/bias.csv --fixed-size=64 --fixed-frac=39  --out-dir=. --name=cordic_bias
```

For the cordic implementation, we need to generate the headers
- bake factor of 3000 used as $exp(-0.001 \times l2Squared) \equiv exp((-0.001 \times l2Squared) / 3)^3 \equiv exp_{bake=3000}(-l2Squared)^3$ 
```bash
exp --iter-end=7 --z-fixed-size=64 --z-fixed-frac=12 --x-fixed-size=64 --x-fixed-frac=12 --bake-factor=3000 --out-file=cordic_power_exp.h
```

And for the negative iteration model
```bash
exp --iter-end=7 --iter-start=-5 --z-fixed-size=64 --z-fixed-frac=10 --x-fixed-size=64 --x-fixed-frac=12 --bake-factor=1000 --out-file=cordic_neg_index_exp.h
```
