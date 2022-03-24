import os
import sys

import pathlib

this_path = pathlib.Path(__file__).resolve().parent
lib_path = this_path.joinpath("lib")
if not lib_path.exists():
    lib_path = this_path.parent.joinpath("lib")
    if not lib_path.exists():
        raise Exception("cannot find library path")

os.add_dll_directory(str(lib_path))
sys.path.insert(0, str(lib_path))

import serial_lib
import stream_lib
import tax_lib
