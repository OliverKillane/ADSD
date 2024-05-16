import math
from dataclasses import dataclass

import numpy as np

from src.lib.qtypes import *
from src.lib.quant import quantise_fn, transform


@dataclass
class CORDICParams:
    iter_start: int
    iter_end: int
    rom: QuantisedData[QuantComplete]
    initial_x: QuantisedData[QuantComplete]
    bake_factor: float
    thetamax: float


def hyper_repetition(iter: int) -> bool:
    """Detect if we need to repeat the step. This follows the pattern next = 3 * prev + 1, etc"""
    if iter == 4:
        return True
    elif (iter - 1) % 3 == 0 and iter > 1:
        return hyper_repetition((iter - 1) // 3)
    else:
        return False


def scaling_factor_K(iters_end: int, iters_start: int) -> float:
    def factor_iter(iter: int) -> float:
        if iter <= 0:
            return math.sqrt(1 - (1 - 2**(iter-2))**2)
        else:
            return math.sqrt(1 - 2**(-2 * iter))
    
    curr = 1.0
    for j in range(iters_start, iters_end + 1):
        curr *= factor_iter(j)
        if hyper_repetition(j):
            curr *= factor_iter(j)
    return curr

def rom_lookup(iter: int) -> float:
    if iter <= 0:
        return math.atanh(1-2**(iter - 2))
    else:
        return math.atanh(2**(-1 * iter))

def rom_params(
    iter_start: int, iter_end: int, bake_factor: float, rom_type: QuantAny, initial_x_type: QuantAny
) -> CORDICParams:
    return CORDICParams(
        iter_start=iter_start,
        iter_end=iter_end,
        rom=transform(
            QuantisedData(
                data=np.array(
                    # We include a dummy zero to ensure the rom used at iter is rom[iter] (not rom[iter - 1])
                    [
                        bake_factor * rom_lookup(j)
                        for j in range(iter_start, iter_end + 1)
                    ]
                ),
                type=rom_type,
            )
        ),
        initial_x=transform(
            QuantisedData(
                data=np.array(1 / scaling_factor_K(iter_end, iter_start)),
                type=initial_x_type,
            )
        ),
        bake_factor=bake_factor,
        thetamax=thetamax(iter_end, iter_start, bake_factor)
    )

def thetamax(iter_end: int, iter_start: int, bake_factor: float) -> float:
    sum = 0.0
    for j in range(iter_start, 1):
        sum += math.atanh(1 - 2**(j - 2))
    sum += math.atanh(2**(-iter_end))
    for j in range(1, iter_end + 1):
        sum += math.atanh(2**(-j))
    return sum * bake_factor
