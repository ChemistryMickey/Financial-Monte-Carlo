import argparse
from dataclasses import fields, MISSING
from typing import Type, TypeVar, Any, get_origin, get_args

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
