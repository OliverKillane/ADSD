import src.qua.cli as cli
import src.qua.header as header
from src.lib.qtypes import *
from src.lib.quant import transform


def pre_transform(d: np.ndarray, transform: str | None) -> np.ndarray:
    import numpy as np

    if transform is not None:
        new_d = eval(transform, {"d": d, "np": np})
        return new_d
    else:
        return d


def main() -> None:
    try:
        args = cli.parse_args()
        data_original = cli.load_csv(args.csv_path)
        data_trans = pre_transform(data_original, args.transform)
        data_quant = transform(QuantisedData(data_trans, type=args.type))
        header.write_header_file(args.header, data_quant, args.name)

    except KeyboardInterrupt:
        print("Interrupted - quitting!")
