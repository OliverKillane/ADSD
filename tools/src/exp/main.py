import src.exp.cli as cli
import src.exp.header as header
import src.exp.params as params


def main() -> None:
    args = cli.parse_args()
    cordic_params = params.rom_params(
        args.iter_start, args.iter_end, args.bake_factor, args.rom_type, args.initial_x_type
    )
    header.write_header(args.out_file, cordic_params)
