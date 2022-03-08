import os

os.add_dll_directory("D:\\boost_1_78_0\\stage\\lib")

import shutil

shutil.copy("build/serial_lib.dll", "./serial_lib.pyd")

import serial_lib

print(serial_lib.parse_data())
