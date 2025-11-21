from io import BytesIO
import base64
import argparse
from dataclasses import fields, MISSING
from typing import Type, TypeVar, Any, get_origin, get_args
from collections.abc import MutableMapping
import matplotlib.pyplot as plt

T = TypeVar("T")


def cli_dataclass(cls: Type[T]) -> Type[T]:
    """
    Decorator that adds a .from_cli() static method to a dataclass.
    """

    def from_cli() -> T:
        parser = argparse.ArgumentParser(description=f"CLI for {cls.__name__}")

        for f in fields(cls):
            arg_name = f"--{f.name}"
            typ = f.type
            kwargs: dict[str, Any] = {}

            # Defaults
            if f.default is not MISSING:
                kwargs["default"] = f.default
                kwargs["help"] = f"(default: {f.default})"
            elif f.default_factory is not MISSING:  # type: ignore
                kwargs["default"] = f.default_factory()
            else:
                kwargs["required"] = True

            # Boolean flags
            if typ is bool:
                kwargs["action"] = (
                    "store_true"
                    if kwargs.get("default", False) is False
                    else "store_false"
                )
                parser.add_argument(arg_name, **kwargs)
                continue

            # list[T]
            if get_origin(typ) is list:
                typ = get_args(typ)[0]
                kwargs["nargs"] = "+"  # accept multiple values

            kwargs["type"] = typ
            parser.add_argument(arg_name, **kwargs)

        args = parser.parse_args()
        return cls(**vars(args))

    cls.from_cli = staticmethod(from_cli)
    return cls


def flatten_dict(
    dict_in: dict[str, any], parent_key: str = "", separator="."
) -> dict[str, any]:
    if dict_in == {} and parent_key != "":
        return {parent_key: {}}

    items = []
    for key, value in dict_in.items():
        new_key = parent_key + separator + key if parent_key else key
        if isinstance(value, MutableMapping):
            items.extend(flatten_dict(value, new_key, separator=separator).items())
        elif isinstance(value, list):
            if value == []:
                items.append((new_key, []))
                continue
            for i, val in enumerate(value):
                new_listkey = f"{new_key}.{i}"
                if isinstance(val, MutableMapping):
                    items.extend(
                        flatten_dict(val, new_listkey, separator=separator).items()
                    )
                else:
                    items.append((new_listkey, val))
        else:
            items.append((new_key, value))
    return dict(items)


def fig2html_str(fig: plt.Figure) -> str:
    buf = BytesIO()
    fig.savefig(buf, format="png")
    buf.seek(0)
    img_base64 = base64.b64encode(buf.read()).decode("ascii")
    return f"""<img src="data:image/png;base64,{img_base64}">"""
