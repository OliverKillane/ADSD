from pathlib import Path

from src.exp.params import CORDICParams
from src.lib.qtypes import *
from src.lib.write import write_data


def write_header(file: Path, params: CORDICParams) -> None:
    if file.exists():
        print(f"Overwriting {file}")

    arr_name = "rom"
    x_name = "initial_x"

    file.parent.mkdir(parents=True, exist_ok=True)
    with open(file, "w") as f:
        print("#pragma once", file=f)
        write_extra(f, arr_name, params.rom.type)
        write_extra(f, x_name, params.initial_x.type)
        print(f"// generated with bake factor {params.bake_factor} for range {params.iter_start} to {params.iter_end}", file=f)
        print(f"#define {arr_name.upper()}_CORDIC_ITER_END {params.iter_end}", file=f)
        print(f"#define {arr_name.upper()}_CORDIC_ITER_START {params.iter_start}", file=f)
        print(f"// The thetamax for this implementation is {params.thetamax}", file=f)
        write_data(f, arr_name, params.rom)
        write_data(f, x_name, params.initial_x)
