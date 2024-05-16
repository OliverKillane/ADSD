from io import TextIOWrapper
from pathlib import Path

from src.lib.qtypes import *
from src.lib.write import write_data


def write_header_file(
    file: Path, qdata: QuantisedData[QuantComplete], name: str
) -> None:

    if file.exists():
        print(f"Overwriting {file}")

    file.parent.mkdir(parents=True, exist_ok=True)
    with open(file, "w") as f:
        # Add include guard
        print("#pragma once", file=f)
        write_extra(f, name, qdata.type)
        write_data(f, name, qdata)
