__lazy_modules__: list[str] = ["typing"]

from typing import Any

def __getattr__(name: str) -> Any: ...
