from argparse import ArgumentParser
from pathlib import Path
from typing import Any

from src.lib.qtypes import *


def with_prefix(arg: str, prefix: str | None) -> str:
    if prefix is not None:
        return f"--{prefix}-{arg}"
    else:
        return f"--{arg}"


def with_prefix_arg(arg: str, prefix: str | None) -> str:
    if prefix is not None:
        return f"{prefix}_{arg}"
    else:
        return arg


def add_quant_args(parser: ArgumentParser, prefix: str | None = None) -> None:

    parser.add_argument(
        with_prefix("bools", prefix), action="store_true", help="use boolean values"
    )
    parser.add_argument(
        with_prefix("floats", prefix),
        action="store_true",
        help="use floats instead of fixed point",
    )
    parser.add_argument(
        with_prefix("doubles", prefix),
        action="store_true",
        help="use doubles instead of fixed point",
    )
    SELECT_MSG: str = (
        "If not set then all possible values will be tested and the one with the flowest mse compared to pre-quantised will be chosen"
    )

    parser.add_argument(
        with_prefix("quantise-frac", prefix),
        type=int,
        default=None,
        help=f"quantise the model weights to a given fraction. {SELECT_MSG}",
    )
    parser.add_argument(
        with_prefix("quantise-size", prefix),
        type=int,
        default=None,
        help="quantise the model weights to a given size.",
    )

    parser.add_argument(
        with_prefix("fixed-size", prefix),
        choices=[8, 16, 32, 64],
        type=int,
        default=None,
        help="Determine the size of C datatype to use for fixed point numbers",
    )
    parser.add_argument(
        with_prefix("fixed-frac", prefix),
        type=int,
        default=None,
        help=f"The number of fractional bits to use with fixed point numbers. {SELECT_MSG}",
    )

    true_false: dict[str, bool] = {"true": True, "false": False}

    parser.add_argument(
        with_prefix("signed", prefix),
        type=lambda x: true_false.get(x),
        choices=true_false.values(),
        default=True,
        help="For quantising and C types, if the data type to be used is signed.",
    )


def get_quant(args: Any, parser: ArgumentParser, prefix: str | None = None) -> QuantAny:

    def get_arg(name: str) -> Any:
        return getattr(args, with_prefix_arg(name, prefix))

    selected = (
        int(get_arg("bools"))
        + int(get_arg("floats"))
        + int(get_arg("doubles"))
        + int(get_arg("quantise_size") is not None)
        + int(get_arg("fixed_size") is not None)
    )
    if selected != 1:
        parser.error("Must select one quantisation method")
    if get_arg("bools") and get_arg("signed") is True:
        parser.error("Cannot set signed for bools")
    if (get_arg("floats") or get_arg("doubles")) and not get_arg("signed"):
        parser.error("Cannot use unsigned floats or doubles")

    if get_arg("floats"):
        return QuantFloat(False)
    elif get_arg("doubles"):
        return QuantFloat(True)
    elif get_arg("bools"):
        return QuantBool()
    elif get_arg("quantise_size") and get_arg("quantise_frac") is None:
        return QuantFixedOptimiseFrac(get_arg("quantise_size"), get_arg("signed"))
    elif get_arg("quantise_size") and get_arg("quantise_frac"):
        return QuantFixed(
            get_arg("quantise_size"), get_arg("quantise_frac"), get_arg("signed")
        )
    elif get_arg("fixed_size") and get_arg("fixed_frac"):
        return QuantCFixed(
            get_arg("fixed_size"), get_arg("fixed_frac"), get_arg("signed")
        )
    elif get_arg("fixed_size") and get_arg("fixed_frac") is None:
        return QuantCFixedOptimiseFrac(get_arg("fixed_size"), get_arg("signed"))
    parser.error("Invalid quantisation method")
