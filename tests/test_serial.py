import os, sys
import socket

hostname = socket.gethostname()
is_test_env = False
if hostname != "user-PC":
    is_test_env = True

import pathlib

bin_path = pathlib.Path(__file__).resolve().parent.parent.joinpath("bin")
if is_test_env:
    os.add_dll_directory(str(bin_path))
    sys.path.insert(0, str(bin_path))

import serial_lib

print(serial_lib.parse_data())
