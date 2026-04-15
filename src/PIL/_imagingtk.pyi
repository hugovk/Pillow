__lazy_modules__: set[str] = {"typing"}

from typing import Any

def __getattr__(name: str) -> Any: ...
