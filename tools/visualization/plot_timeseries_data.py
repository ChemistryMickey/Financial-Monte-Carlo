from pathlib import Path
import pandas as pd
import matplotlib.pyplot as plt
from dataclasses import dataclass
from tools.utils import cli_dataclass
from tools.visualization.plot_dispersions import plot_dispersions_to_html
from tools.visualization.plot_mc_kpis import plot_mc_kpis_to_html_str
from tools.visualization.plot_mc_runs import (
    plot_mc_runs_to_html_str,
)


@cli_dataclass
@dataclass
class PlotTimeSeriesArgs:
    output_dir: str
    title: str = "Time Series Plot"
    x_label: str = "Date [-]"
    y_label: str = "Value [$]"
    save_dir: str = None
    use_logy: bool = False
    plot_all_subruns: bool = False


def plot_time_series_from_path(args: PlotTimeSeriesArgs):
    out_dir = "output" / Path(args.output_dir)
    runs = list(out_dir.glob("RUN_*"))
    if len(runs) == 0:
        # Then this is a single run
        df = pd.read_csv(
            out_dir / "timeseries_data.csv",
            float_precision="round_trip",
            index_col="Date",
        )
        plot_single_time_series(
            df,
            args.title,
            args.x_label,
            args.y_label,
            False,
            "output" / Path(args.save_dir) / "all_signals.png"
            if args.save_dir is not None
            else None,
            args.use_logy,
        )

        for col in df.columns:
            plot_single_time_series(
                df[col],
                args.title,
                args.x_label,
                col,
                False,
                "output" / Path(args.save_dir) / f"{col}.png"
                if args.save_dir is not None
                else None,
                False,
            )
    else:
        viz_dir = Path("output") / args.save_dir
        viz_dir.mkdir(parents=True, exist_ok=True)

        report_html_str = """
<html>
<body>
<h1>Monte Carlo Report</h1>
"""

        timeseries_data_exist = list(runs[0].glob("timeseries*.csv")).__len__() != 0
        if timeseries_data_exist:
            report_html_str = plot_mc_kpis_to_html_str(out_dir, report_html_str)
            report_html_str = plot_mc_runs_to_html_str(out_dir, report_html_str)

        report_html_str = plot_dispersions_to_html(out_dir, report_html_str)

        report_html_str += """
</body>
</html>
"""
        (viz_dir / "report.html").write_text(report_html_str)


def plot_single_time_series(
    df: pd.DataFrame,
    title: str = "Time Series Plot",
    x_label: str = "Date [-]",
    y_label: str = "Value [$]",
    show_plot: bool = True,
    save_path: str | Path | None = None,
    use_logy: bool = False,
):
    # Create a figure with appropriate size
    plt.figure(figsize=(16, 8))

    # Plot each column as a line on the same axes
    df.plot(ax=plt.gca())

    # Add labels and title
    plt.xlabel(x_label)
    plt.ylabel(y_label)
    plt.title(title)
    if use_logy:
        plt.yscale("log")

    # Improve readability with grid, rotated x-ticks, and legend
    plt.grid(True)
    plt.xticks(rotation=30)
    plt.legend(loc="upper right")

    # Save plot if requested
    if save_path is not None:
        save_path.parent.mkdir(parents=True, exist_ok=True)
        plt.savefig(save_path, dpi=250)

    # Show plot if requested
    if show_plot:
        plt.show()


def main():
    args: PlotTimeSeriesArgs = PlotTimeSeriesArgs.from_cli()
    plot_time_series_from_path(args)


if __name__ == "__main__":
    main()
