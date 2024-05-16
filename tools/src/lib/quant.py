import time
from typing import Any, Callable

from fxpmath import Fxp  # type: ignore

from src.lib.qtypes import *


def find_best_quantise(qdata: QuantisedData[QuantOptimise]) -> QuantComplete:
    """
    Quantises the data to a given fixed point format where (bits + frac = size)
    using the smallest MSE.

    Runs for CFixed and QuantFixed
    """

    def find_frac(size: int, signed: bool, data: np.ndarray) -> int:
        print("BEWARE: optimising this will take a long time")
        min_mse: tuple[float, int] | None = None
        for frac in range(0, size + (0 if signed else 1)): # sign takes up 1 bit
            print(
                f"Testing with size: {size} and frac: {frac}", end=" ... ", flush=True
            )

            def get_err(x: np.float64) -> np.float64:
                fp = Fxp(x, signed, size, frac)
                return np.float64((x - fp()) ** 2)

            get_err_vec = np.vectorize(get_err)

            start = time.time()
            print(f"Computing error for {len(data)} rows", end=" ... ", flush=True)
            error = get_err_vec(data).sum()
            print(f"error is {error} after {time.time() - start:.2f}s")
            match min_mse:
                case None:
                    min_mse = (error, frac)
                case (mse, _) if error < mse:
                    min_mse = (error, frac)

        assert min_mse is not None
        _, frac = min_mse
        print(f"Choosing fractional bits: {frac}")
        return frac

    data = qdata.data

    match qdata.type:
        case QuantFixedOptimiseFrac(size, signed):
            frac = find_frac(size, signed, data)
            return QuantFixed(size, frac, signed)
        case QuantCFixedOptimiseFrac(size, signed):
            frac = find_frac(size, signed, data)
            return QuantCFixed(size, frac, signed)
    assert False, "unreachable"


def quantise_fn(q_type: QuantComplete) -> Callable[[float], QuantComplete]:
    match q_type:
        case QuantFloat(True):

            def quantise(x: np.float64) -> Any:
                return np.float64(x)

        case QuantFloat(False):

            def quantise(x: np.float64) -> Any:
                return np.float32(x)

        case QuantBool():

            def quantise(x: np.float64) -> Any:
                return int(bool(x))

        case QuantCFixed(size, frac, signed) | QuantFixed(size, frac, signed):

            def quantise(x: np.float64) -> Any:
                return round(Fxp(x, signed, size, frac).val[()])

        case _:
            assert False, "unreachable"

    return quantise


def quantise(qdata: QuantisedData[QuantComplete]) -> QuantisedData[QuantComplete]:
    """Given some size and fractional, produce a fixed point representation"""

    quantise = quantise_fn(qdata.type)

    print(f"Quantising {qdata.data.shape} rows", end=" ... ", flush=True)
    start = time.time()
    quantise_vec = np.vectorize(quantise)
    res = QuantisedData(quantise_vec(qdata.data), qdata.type)
    print(f"Complete in {time.time() - start:.2f}s")
    return res


def transform(qdata: QuantisedData[QuantAny]) -> QuantisedData[QuantComplete]:
    """If the frac needs to be inferred, infer it"""
    match qdata.type:
        case QuantFixedOptimiseFrac(_) | QuantCFixedOptimiseFrac(_):
            optimal = find_best_quantise(QuantisedData(qdata.data, qdata.type))
            return quantise(QuantisedData(qdata.data, optimal))
        case _:
            return quantise(QuantisedData(qdata.data, qdata.type))
