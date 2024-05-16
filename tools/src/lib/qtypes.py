from dataclasses import dataclass
from io import TextIOWrapper
from typing import Generic, TypeVar

import numpy as np


@dataclass
class QuantBool:
    pass


@dataclass
class QuantFloat:
    double: bool


@dataclass
class QuantFixed:
    size: int
    frac: int
    signed: bool


@dataclass
class QuantFixedOptimiseFrac:
    size: int
    signed: bool


@dataclass
class QuantCFixed:
    size: int
    frac: int  # INV: frac <= bytes * 8
    signed: bool


@dataclass
class QuantCFixedOptimiseFrac:
    size: int
    signed: bool


QuantComplete = QuantFixed | QuantFloat | QuantBool | QuantCFixed
QuantOptimise = QuantFixedOptimiseFrac | QuantCFixedOptimiseFrac
QuantAny = QuantComplete | QuantOptimise

T = TypeVar("T")


@dataclass
class QuantisedData(Generic[T]):
    data: np.ndarray
    type: T


def get_type(type: QuantComplete) -> str:
    match type:
        case QuantFloat(True):
            return "double"
        case QuantFloat(False):
            return "float"
        case QuantFixed(size, frac, signed):
            # Decided against using the ap_fixed types to allow programmer
            # complete control over implementation
            return f"ap_{'' if signed else 'u'}int<{size}> /* frac={frac} */"
        case QuantBool():
            return "bool"
        case QuantCFixed(size, frac, signed):
            return f"{'' if signed else 'u'}int{size}_t /* frac={frac} */"
    assert False, "Unreachable"


def write_extra(f: TextIOWrapper, name: str, type: QuantComplete):
    match type:
        case QuantCFixed(_, frac):
            print("#include <stdint.h>", file=f)
            print(f"#define {name.upper()}_FRAC_BITS {frac}", file=f)
            print(f"#define {name.upper()}_FRAC_DIV {2**frac}", file=f)
