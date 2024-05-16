from io import TextIOWrapper

from src.lib.qtypes import *


def write_array(f: TextIOWrapper, data: np.ndarray, end: bool):
    match data.shape:
        case ():
            print(data[()], file=f)
        case (_,):
            print("{", file=f)
            for val in data:
                print(f"{val},", file=f)
            if end:
                print("}", file=f)
            else:
                print("},", file=f)
        case _:
            print("{", file=f)
            for row in data:
                write_array(f, row, False)
            print("}", file=f)
            if not end:
                print(",", file=f)
    if end:
        print(";", file=f)


def write_data(f: TextIOWrapper, name: str, qdata: QuantisedData[QuantComplete]):
    data_type: str = get_type(qdata.type)

    # Write the sizes
    for dim, size in enumerate(qdata.data.shape):
        print(f"#define {name.upper()}_DIM_{dim} {size}", file=f)

    print(f"typedef {data_type} {name}_t;", file=f)
    # declaration string
    arr_decl = "".join(f"[{d}]" for d in qdata.data.shape)
    print(f"const static {name}_t {name}{arr_decl} = ", end="", file=f)

    write_array(f, qdata.data, True)
    print(f"{data_type} {name}{arr_decl} ({qdata.data.shape} elements)")
