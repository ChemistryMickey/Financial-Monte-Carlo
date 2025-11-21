from pathlib import Path
import pandas as pd
import matplotlib.pyplot as plt
import json
from tqdm import tqdm
from tools.utils import flatten_dict, fig2html_str


def load_json(path: Path) -> dict:
    return json.load(path.open())


def plot_dispersions_to_html(
    out_dir: Path, report_html_str: str, n_bins: int = 50
) -> str:
    runs = sorted(list(out_dir.glob("RUN_*")))
    configs = {
        run.stem: flatten_dict(load_json(run / "monte_config.lock")) for run in runs
    }

    df = pd.DataFrame.from_dict(configs).T

    report_html_str += """
<h2> Dispersions </h2>
<details>
"""

    for col in tqdm(df.columns, ncols=150, desc="Plotting Dispersions"):
        try:
            # Create a figure with appropriate size
            fig = plt.figure(figsize=(16, 8))

            # Plot each column as a line on the same axes
            df[col].hist(bins=n_bins)

            # Add labels and title
            plt.xlabel(col)
            plt.ylabel("Number of Instances [-]")
            plt.title(f"{col} Dispersion")

            # Improve readability with grid, rotated x-ticks, and legend
            plt.xticks(rotation=30)

            report_html_str += fig2html_str(fig)

            plt.close()
        except:  # noqa: E722
            print(f"Failed to plot {col}")

    report_html_str += """</details>"""

    return report_html_str
