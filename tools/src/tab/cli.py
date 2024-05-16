from argparse import ArgumentParser
from dataclasses import dataclass

from src.lib.qcli import *
from src.lib.qtypes import *

PROG_NAME = "exp"


@dataclass
class Args:
    # INV: range_start < range_end
    range_start: float
    range_end: float

    # the expression, taking x as input, with math available
    expr: str

    name: str

    type: QuantAny
    index_frac_bits: int

    out_dir: Path


def parse_args():
    parser = ArgumentParser(
        prog=PROG_NAME,
        description="Generate header files tabulating values for floating point functions",
        epilog="Have a wonderful day!",
    )

    parser.add_argument(
        "--range-start",
        required=True,
        type=float,
        help="The start of the range to calculate the exp function",
    )

    parser.add_argument(
        "--range-end",
        required=True,
        type=float,
        help="The end of the range to calculate the exp function",
    )

    parser.add_argument(
        "--index-frac",
        required=True,
        type=int,
        help="The number of fraction bits to access by index array[int.frac]",
    )

    parser.add_argument(
        "--expr",
        required=True,
        type=str,
        help="The expression to tabulate. Based on parameter x, with math available. for example: `-0.001 * math.exp(x)`",
    )

    parser.add_argument(
        "--name",
        required=True,
        type=str,
        help="The name to use (for array and header file)",
    )

    parser.add_argument(
        "--out-dir",
        required=True,
        type=Path,
        help="The directory to write the header file to",
    )

    add_quant_args(parser)

    args = parser.parse_args()

    if args.range_start >= args.range_end:
        parser.error("range-start must be less than range-end")

    return Args(
        range_start=args.range_start,
        range_end=args.range_end,
        expr=args.expr,
        name=args.name,
        type=get_quant(args, parser),
        index_frac_bits=args.index_frac,
        out_dir=args.out_dir,
    )
