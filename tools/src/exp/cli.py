from src.lib.qcli import *
from src.lib.qtypes import *

PROG_NAME: str = "exp"
INITIAL_X_TYPE = "x"
ROM_TYPE = "z"


@dataclass
class Args:
    rom_type: QuantAny
    initial_x_type: QuantAny
    out_file: Path
    iter_start: int
    iter_end: int
    bake_factor: float


def parse_args() -> Args:
    parser = ArgumentParser(
        prog=PROG_NAME,
        description="Generate a header of the parameters needed for cordic exp",
        epilog="Have a wonderful day!",
    )

    parser.add_argument(
        "--iter-end",
        required=True,
        type=int,
        help="The number of iterations to use for the CORDIC algorithm",
    )
    
    parser.add_argument(
        "--iter-start",
        default=1,
        type=int,
        help="The number of iterations to use for the CORDIC algorithm",
    )

    parser.add_argument(
        "--bake-factor",
        default=1.0,
        type=float,
        help="If doing cordic for `exp(1/a * x)`, we can bake the factor `a` into exp as `exp_a(x)`",
    )
    add_quant_args(parser, INITIAL_X_TYPE)
    add_quant_args(parser, ROM_TYPE)

    parser.add_argument(
        "--out-file",
        required=True,
        type=Path,
        help="The location to place the header",
    )

    args = parser.parse_args()

    return Args(
        rom_type=get_quant(args, parser, ROM_TYPE),
        initial_x_type=get_quant(args, parser, INITIAL_X_TYPE),
        out_file=args.out_file,
        iter_start=args.iter_start,
        iter_end=args.iter_end,
        bake_factor=args.bake_factor,
    )
