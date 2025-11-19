from pathlib import Path
import pandas as pd
import matplotlib.pyplot as plt
from dataclasses import dataclass
from tools.utils import cli_dataclass


@cli_dataclass
@dataclass
class PlotTimeSeriesArgs:
    output_dir: str
    title: str = "Time Series Plot"
    x_label: str = "Date [-]"
    y_label: str = "Value [$]"
    save_dir: str = None
    use_logy: bool = False


def plot_time_series_from_path(
    output_directory: str,
    title: str = "Time Series Plot",
    x_label: str = "Date [-]",
    y_label: str = "Value [$]",
    show_plot: bool = True,
    save_dir: str | Path | None = None,
    use_logy: bool = False,
):
    out_dir = "output" / Path(output_directory)
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
            title,
            x_label,
            y_label,
            show_plot,
            "output" / Path(save_dir) / "all_signals.png"
            if save_dir is not None
            else None,
            use_logy,
        )

        for col in df.columns:
            plot_single_time_series(
                df[col],
                title,
                x_label,
                col,
                False,
                "output" / Path(save_dir) / f"{col}.png"
                if save_dir is not None
                else None,
                False,
            )
    else:
        # plot_monte_carlo(
        #     out_dir,
        #     runs,
        # )
        msg = "Plotting Monte Carlos not yet implemented!"
        raise Exception(msg)


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
    plot_time_series_from_path(
        args.output_dir,
        args.title,
        args.x_label,
        args.y_label,
        False,
        args.save_dir,
        args.use_logy,
    )


if __name__ == "__main__":
    main()
