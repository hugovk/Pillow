"""
Print out some handy system info like Travis CI does.

This sort of info is missing from GitHub Actions.

Requested here:
https://github.com/actions/virtual-environments/issues/79
"""

from __future__ import annotations

import os
import platform
import sys

print("Build system information")
print()

print("sys.version\t\t", sys.version.split("\n"))
print("os.name\t\t\t", os.name)
print("sys.platform\t\t", sys.platform)
print("platform.system()\t", platform.system())
print("platform.machine()\t", platform.machine())
print("platform.platform()\t", platform.platform())
print("platform.version()\t", platform.version())
print("platform.uname()\t", platform.uname())
if sys.platform == "darwin":
    print("platform.mac_ver()\t", platform.mac_ver())
if sys.version_info >= (3, 14):
    print("sys._jit.is_available()\t", sys._jit.is_available())
    print("sys._jit.is_enabled()\t", sys._jit.is_enabled())
    print("sys._jit.is_active()\t", sys._jit.is_active())
