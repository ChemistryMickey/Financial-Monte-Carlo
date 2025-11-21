from pathlib import Path
import pandas as pd
import numpy as np
from tqdm import tqdm
import matplotlib.pyplot as plt
from tools.utils import fig2html_str


def plot_mc_runs_to_html_str(out_dir: Path, report_html_str: str) -> str:
    runs = sorted(list(out_dir.glob("RUN_*")))
    timeseries_data = {
        str(run.stem): pd.read_csv(
            run / "timeseries_data.csv", float_precision="round_trip", index_col="Date"
        )
        for run in runs
    }
    cols = list(timeseries_data.values())[0].columns

    report_html_str += """
<h2> Run Subsets and Statistical Enclosures </h2>
<details>
"""
    run_inds = sorted([int(run.stem.split("_")[1]) for run in runs])
    n_runs_to_plot = 20 if len(run_inds) > 20 else len(run_inds) - 1
    cols_to_plot = sorted(
        {
            f"RUN_{i:09}"
            for i in np.linspace(0, len(run_inds) - 1, n_runs_to_plot).astype(int)
        }
    )
    for col in tqdm(cols, ncols=150, desc="Plotting Run Subsets"):
        report_html_str += f"""
<h3> {col} </h3>
<details>
"""
        try:
            df = pd.concat(
                {run: subdf[col] for run, subdf in timeseries_data.items()}, axis=1
            )

            # First, plot a subset of the runs for that column
            fig = plt.figure(figsize=(16, 8))
            plt.plot(df[cols_to_plot], label=cols_to_plot)
            plt.title(f"{col} Run subset")
            plt.xlabel("Date [-]")
            plt.ylabel(col)
            if not any(df < 0):
                plt.yscale("log")
            inds_to_plot = sorted(
                {df.index[i] for i in np.linspace(0, len(df.index) - 1, 20).astype(int)}
            )
            plt.xticks(inds_to_plot, rotation=30)
            plt.legend()

            report_html_str += fig2html_str(fig)
            plt.close()

            # Then, plot the statistical enclosures for that column
            fig = plt.figure(figsize=(16, 8))
            df = df.T

            stats = pd.DataFrame(
                {
                    "min": df.min(),
                    "25%": df.quantile(0.25),
                    "50%": df.quantile(0.50),
                    "75%": df.quantile(0.75),
                    "max": df.max(),
                }
            )
            for stat in stats.columns:
                plt.plot(stats.index, stats[stat], label=stat)

            plt.title(f"{col} Statistical Enclosure Plot")
            plt.xlabel("Date [-]")
            plt.xticks(inds_to_plot, rotation=30)
            plt.ylabel(col)
            plt.grid(True)
            plt.legend()

            report_html_str += fig2html_str(fig)
            plt.close()

            # Do it again but with a logy scale
            fig = plt.figure(figsize=(16, 8))
            for stat in stats.columns:
                plt.plot(stats.index, stats[stat], label=stat)

            plt.title(f"{col} Statistical Enclosure Plot")
            plt.xlabel("Date [-]")
            plt.xticks(inds_to_plot, rotation=30)
            plt.yscale("log")
            plt.ylabel(col)
            plt.grid(True)
            plt.legend()

            report_html_str += fig2html_str(fig)
            plt.close()

        except Exception as e:  # noqa: E722
            print(f"\nFailed to plot {col}: {e}")

        report_html_str += "</details>"

    report_html_str += """</details>"""

    return report_html_str
