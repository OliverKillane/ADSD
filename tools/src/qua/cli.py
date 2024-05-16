from argparse import ArgumentParser
from dataclasses import dataclass
from pathlib import Path

import numpy as np

from src.lib.qcli import *
from src.lib.qtypes import *

PROG_NAME = "qua"


@dataclass
class Args:
    csv_path: Path
    type: QuantAny
    transform: str | None
    header: Path
    name: str


def parse_args() -> Args:
    parser = ArgumentParser(
        prog=PROG_NAME,
        description="Converts csvs of values into quantisized values",
        epilog="Have a wonderful day!",
    )

    parser.add_argument(
        "--csv-path",
        type=Path,
        required=True,
        help="Path to the csv file",
    )

    parser.add_argument(
        "--transform",
        type=str,
        default=None,
        help="Apply a transformation to the data before quantising it (e.g. reshaping the array)",
    )

    # Quantise arguments
    add_quant_args(parser)

    parser.add_argument(
        "--out-dir",
        type=Path,
        required=True,
        help="the header file name to write the weights to",
    )
    parser.add_argument(
        "--name",
        type=str,
        required=True,
        help="the name of the array in the header file, and the header file itself",
    )

    args = parser.parse_args()

    return Args(
        csv_path=args.csv_path,
        type=get_quant(args, parser),
        transform=args.transform,
        header=args.out_dir / f"{args.name}.h",
        name=args.name,
    )


def load_csv(path: Path) -> np.ndarray:
    """Load csv and convert to a 1d array"""
    return np.genfromtxt(path, delimiter=",")
