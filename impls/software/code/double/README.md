## Doubles
A naive implementation using double precision floating point numbers.
```bash
qua --csv-path=../../../../models/0_1/weights/svs.csv       --doubles --out-dir=. --name=svs      
qua --csv-path=../../../../models/0_1/weights/alphas.csv    --doubles --out-dir=. --name=alphas   
qua --csv-path=../../../../models/0_1/weights/bias.csv      --doubles --out-dir=. --name=bias     
qua --csv-path=../../../../models/0_1/weights/test_data.csv --doubles --out-dir=. --name=test_data
```

For the cordic implementation we can use:
```bash
exp --iter-end=8 --doubles --bake-factor=3000 --out-dir=.
```