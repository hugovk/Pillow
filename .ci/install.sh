#!/bin/bash

aptget_update()
{
    if [ ! -z $1 ]; then
        echo ""
        echo "Retrying apt-get update..."
        echo ""
    fi
    output=`sudo apt-get update 2>&1`
    echo "$output"
    if [[ $output == *[WE]:\ * ]]; then
        return 1
    fi
}
if [[ $(uname) != CYGWIN* ]]; then
    aptget_update || aptget_update retry || aptget_update retry
fi

set -e

if [[ $(uname) != CYGWIN* ]]; then
    sudo time apt-get -qq install libfreetype6-dev liblcms2-dev python3-tk\
                             ghostscript libffi-dev libjpeg-turbo-progs libopenjp2-7-dev\
                             cmake meson imagemagick libharfbuzz-dev libfribidi-dev\
                             sway wl-clipboard libopenblas-dev
fi

time python3 -m pip install --upgrade pip
time python3 -m pip install --upgrade wheel
time PYTHONOPTIMIZE=0 python3 -m pip install cffi
time python3 -m pip install coverage
time python3 -m pip install defusedxml
time python3 -m pip install olefile
time python3 -m pip install -U pytest
time python3 -m pip install -U pytest-cov
time python3 -m pip install -U pytest-timeout
time python3 -m pip install pyroma

if [[ $(uname) != CYGWIN* ]]; then
    time python3 -m pip install numpy

    # PyQt6 doesn't support PyPy3
    if [[ $GHA_PYTHON_VERSION == 3.* ]]; then
        sudo time apt-get -qq install libegl1 libxcb-cursor0 libxcb-icccm4 libxcb-image0 libxcb-keysyms1 libxcb-randr0 libxcb-render-util0 libxcb-shape0 libxkbcommon-x11-0
        time python3 -m pip install pyqt6
    fi

    # webp
    pushd depends && time ./install_webp.sh && popd

    # libimagequant
    pushd depends && time ./install_imagequant.sh && popd

    # raqm
    pushd depends && time ./install_raqm.sh && popd

    # extra test images
    pushd depends && time ./install_extra_test_images.sh && popd
else
    cd depends && time ./install_extra_test_images.sh && cd ..
fi
