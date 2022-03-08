import os, sys
import pathlib
if not os.path.exists("serial_lib.pyd"):
    bin_path = pathlib.Path(__file__).resolve().parent.parent.joinpath("bin")
    os.add_dll_directory(str(bin_path))
    sys.path.insert(0, str(bin_path))

import serial_lib

print(serial_lib.parse_data())
