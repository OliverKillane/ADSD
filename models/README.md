## SVM models
To train and save a new model:
```bash
svm --input=../data/train.csv --test train --save=0_1/model --labels 0 1
svm --input=../data/train.csv --output 0_1/weights --test load --model=0_1/model
```

The [original weights](./0_1_original/) are extracted from department provided matlab files.
