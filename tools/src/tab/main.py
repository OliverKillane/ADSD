import src.tab.cli as cli
import src.tab.header as header
import src.tab.tables as tables


def main():
    args = cli.parse_args()
    data = tables.table_quant(
        *tables.get_vals(
            args.range_start, args.range_end, args.index_frac_bits, args.expr
        ),
        args.type,
    )
    header.write_header(args.out_dir / f"{args.name}.h", data, args.name, args.expr)
