from dataclasses import dataclass
from pathlib import Path

import numpy as np
import pandas as pd
from sklearn.model_selection import train_test_split  # type: ignore
from sklearn.preprocessing import scale  # type: ignore


@dataclass
class DataSet:
    labels: np.ndarray
    inputs: np.ndarray


# INV: images are of size 28 * 28 (consts.IMAGE_SIZE)
def load_dataset(training_data: Path) -> tuple[DataSet, DataSet]:
    """
    1. reads a csv of form:
        label   , pixel0 , pixel1 , ..., pixel783, pixel783
        <digit> , <0-255>, <0-255>, ..., <0-255> , <0-255>
      where each row is a 28x28 image representing the digit in the label column
    2. then scales each pixel down to a float between 0 and 1
    3. Splits the data into training (70%) and testing (30%) sets
    """

    print(f"reading training data from {training_data}")

    data = pd.read_csv(training_data)

    y = data["label"]
    x = data.drop(columns="label") / 255.0

    # convert into ndarrays
    xScaled = scale(x)
    y_array = np.array(y)
    xTrain, xTest, yTrain, yTest = train_test_split(
        xScaled, y_array, test_size=0.3, train_size=0.7, random_state=10
    )

    return DataSet(labels=yTest, inputs=xTest), DataSet(labels=yTrain, inputs=xTrain)


def filter_labels(
    test: DataSet,
    train: DataSet,
    labels: set[int],
) -> tuple[DataSet, DataSet]:
    """
    Extracts only rows for the labels from the dataset.
    - e.g. filter_labels(test, train, {2, 7}) will return only the rows where the
           label is 2 or 7
    """

    def extract_class(data: DataSet, labels: set[int]) -> DataSet:
        indices = [i for i, x in enumerate(data.labels.tolist()) if x in labels]
        return DataSet(labels=data.labels[indices], inputs=data.inputs[indices])

    return extract_class(test, labels), extract_class(train, labels)
