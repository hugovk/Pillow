import sys

import PIL
import pytest


def test_deprecated_pillow_version():
    if sys.version_info >= (3, 7):
        expected = DeprecationWarning
    else:
        expected = None

    with pytest.warns(expected):
        version = PIL.PILLOW_VERSION

    assert version == PIL.__version__
