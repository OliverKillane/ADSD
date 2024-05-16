from math import trunc

import numpy as np

from src.lib.qtypes import *
from src.lib.quant import *


@dataclass
class TableRange:
    start: int
    end: int
    frac_bits: int

    def get_gloat_range(self) -> tuple[float, float]:
        factor = 2**self.frac_bits
        return (float(self.start) / factor, float(self.end) / factor)


def apply_expr(x: float, expr: str) -> float:
    import math  # required so math functions are available for eval

    val = eval(expr, {'x' : x, 'math' : math})
    return float(val)


@dataclass
class Table:
    indices: np.ndarray
    vals: np.ndarray
    type: QuantComplete
    range: TableRange


def get_vals(
    range_start: float, range_end: float, frac_bits: int, expr: str
) -> tuple[np.ndarray, np.ndarray, TableRange]:
    """Outputs a vector [(val, exp(val)), ...]"""
    shift_factor = 2**frac_bits
    quant_start = trunc(range_start * shift_factor)
    quant_end = trunc(range_end * shift_factor)

    indices = np.arange(quant_start, quant_end + 1, 1)

    def convert(x: np.int64) -> tuple[float, float]:
        val = float(x) / shift_factor
        exp_val = apply_expr(val, expr)
        return (val, exp_val)

    convert_vec = np.vectorize(convert)
    vecs = convert_vec(indices)
    return vecs[0], vecs[1], TableRange(quant_start, quant_end, frac_bits)

def trim_table(
    tab: Table
) -> Table:
    # limit the range of the start and end for repeating values
    
    # trim the front
    front_index = 0
    while front_index < len(tab.vals) - 1 and tab.vals[front_index] == tab.vals[front_index + 1]:
        front_index += 1
    
    # trim the back
    back_index = len(tab.vals)
    while back_index > 2 and tab.vals[back_index - 1] == tab.vals[back_index - 2]:
        back_index -= 1
    remove_from_back = len(tab.vals) - back_index
    
    return Table(
        tab.indices[front_index:back_index], 
        tab.vals[front_index:back_index],
        tab.type, 
        TableRange(
            tab.range.start + front_index, 
            tab.range.end - remove_from_back,
            tab.range.frac_bits
        ),
    )
        
def table_quant(
    inputs: np.ndarray, vals: np.ndarray, tablerange: TableRange, type: QuantAny
) -> Table:
    qdata = QuantisedData(vals, type)
    qtransformed = transform(qdata)
    table = Table(inputs, qtransformed.data, qtransformed.type, tablerange)
    return trim_table(table)