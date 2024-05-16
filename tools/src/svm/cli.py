from argparse import ArgumentParser, Namespace
from dataclasses import dataclass
from pathlib import Path

import numpy as np


@dataclass
class Train:
    save: Path | None  # Train a new model, optionally save
    labels: list[int]


@dataclass
class Load:
    saved: Path  # load a new model


@dataclass
class Args:
    source: Train | Load
    train_test_data: Path
    test: bool
    output: Path | None


def parse_args() -> Args:
    LABELS_OPT = "--labels"

    parser = ArgumentParser(
        prog="svm",
        description=f"""
Generate weights for the SVM ADSD coursework.

Models can be trained or loaded, and classify between two different characters selected by {LABELS_OPT}

Weights are then output to a directory of csv files.
""",
        epilog="Have a wonderful day!",
    )

    parser.add_argument(
        "--input",
        type=Path,
        required=True,
        help="input data to train or test the model",
    )
    parser.add_argument("--output", type=Path, help="output the weights to a model")
    parser.add_argument(
        "--test", action="store_true", help="input data to train or test the model"
    )

    model_parser = parser.add_subparsers(dest="model_parse", help="Model Sources")

    train_parser = model_parser.add_parser(
        name="train", description="train from mnist input data"
    )
    train_parser.add_argument(
        "--save", type=Path, default=None, required=False, help="save the model here"
    )
    train_parser.add_argument(
        LABELS_OPT, nargs=2, required=True, help="the labels/digits to be used"
    )

    load_parser = model_parser.add_parser(
        name="load", description="load a pre-trained model"
    )
    load_parser.add_argument(
        "--model", type=Path, required=True, help="the previously saved model file"
    )

    args = parser.parse_args()

    def get_source(args: Namespace) -> Load | Train:
        if args.model_parse == "load":
            return Load(saved=args.model)
        elif args.model_parse == "train":
            return Train(save=args.save, labels=[int(i) for i in args.labels])
        else:
            parser.error("Must either train or load a model")

    return Args(
        source=get_source(args),
        train_test_data=args.input,
        test=args.test,
        output=args.output,
    )


def output_values(path: Path, outputs: dict[str, np.ndarray]) -> None:
    path.mkdir(parents=True, exist_ok=True)
    for name, data in outputs.items():
        filepath = path / f"{name}.csv"
        if filepath.exists():
            print(f"Overwriting {filepath}")
        else:
            print(f"Saving {filepath}")
        np.savetxt(filepath, data, delimiter=",")
