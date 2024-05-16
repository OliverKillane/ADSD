import pickle
import time
from dataclasses import dataclass
from pathlib import Path

import numpy as np
from sklearn.metrics import accuracy_score  # type: ignore
from sklearn.svm import SVC  # type: ignore

import src.svm.extract as extract
from src.svm.consts import IMAGE_SIZE


def train_model(training: extract.DataSet) -> SVC:
    """Train on the given model, as we do binary classification, only 2 labels
    can be present
    """
    model = SVC(kernel="rbf", gamma=0.001, C=10)
    print(f"Training model", end=" ... ", flush=True)
    start = time.time()
    model.fit(training.inputs, training.labels)
    print(f"took {time.time() - start:.2f}s")
    return model


def test_model(testing: extract.DataSet, model: SVC) -> float:
    """Infer on a given model and return the accuracy metric"""
    print(f"Runing inference with model", end=" ... ", flush=True)
    start = time.time()
    pred = model.predict(testing.inputs)
    accuracy = float(accuracy_score(testing.labels, pred))
    print(f"took {time.time() - start:.2f}s for {accuracy*100:.2f}% accuracy")
    return accuracy


@dataclass
class SavedModel:
    model: SVC
    labels: list[int]


def save_model(model: SavedModel, path: Path) -> None:
    path.parent.mkdir(parents=True, exist_ok=True)
    with open(path, "wb") as f:
        pickle.dump(model, f)
    print(f"Saved model to {path}")


def load_model(path: Path) -> SavedModel:
    print(f"Loading model from {path}")
    with open(path, "rb") as f:
        return pickle.load(f)  # type: ignore


def generate_model_quants(model: SVC, test: extract.DataSet) -> dict[str, np.ndarray]:
    return {
        "svs": model.support_vectors_,
        "bias": model.intercept_,
        "alphas": model.dual_coef_,
        "test_data": test.inputs,
        "ground_truth": test.labels,
    }
