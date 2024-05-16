from pathlib import Path

import numpy as np

from src.lib.qtypes import *
from src.tab.tables import Table, TableRange


def write_access(f: TextIOWrapper, trange: TableRange, type: str, name: str) -> None:
    size = trange.end - trange.start
    print(
        f"""
static inline {type} index_{name}(size_t index) {{
    // align range to start of array
    
    if (index < {trange.start}) {{
        index = 0;
    }} else if (index > {trange.end}) {{ 
        index = {size - 1};
    }}else {{
        index -= {trange.start};  
    }}
    
    return {name}[index];
}}""",
        file=f,
    )


def write_header(file: Path, tab: Table, name: str, expr: str) -> None:
    if file.exists():
        print(f"Overwriting {file}")

    file.parent.mkdir(parents=True, exist_ok=True)
    with open(file, "w") as f:
        dtype = get_type(tab.type)
        length = tab.indices.size

        print("#pragma once", file=f)
        write_extra(f, name, tab.type)
        print("#include <stddef.h>", file=f)
        print(f"typedef {dtype} {name}_t;", file=f)
        print(f"#define {name.upper()}_LEN {length}", file=f)
        fstart, fend = tab.range.get_gloat_range()
        print(f"// {name} defined for table covering {fstart} to {fend}", file=f)
        print(f"const static {name}_t {name}[{length}] = {{", file=f)
        for val, inp in zip(tab.vals, tab.indices):
            print(f"    {val}, // = {expr} where x = {inp}", file=f)
        print("};", file=f)

        write_access(f, tab.range, dtype, name)

    print(f"{dtype} {name} to {file}")
