from __future__ import annotations

import pytest

from PIL import FontFile, Image

TYPE_CHECKING = False
if TYPE_CHECKING:
    from pathlib import Path


def test_compile() -> None:
    font = FontFile.FontFile()
    font.glyph[0] = ((0, 0), (0, 0, 0, 0), (0, 0, 0, 1), Image.new("L", (0, 0)))
    font.compile()
    assert font.ysize == 1

    font.ysize = 2
    font.compile()

    # Assert that compiling again did not change anything
    assert font.ysize == 2


def test_save(tmp_path: Path) -> None:
    tempname = str(tmp_path / "temp.pil")

    font = FontFile.FontFile()
    with pytest.raises(ValueError):
        font.save(tempname)
