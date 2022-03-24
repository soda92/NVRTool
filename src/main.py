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

from tabs.MainWindow import MainWindow
from PyQt6 import QtWidgets
import sys
import json
from pathlib import Path
import logging

if __name__ == "__main__":
    app = QtWidgets.QApplication(sys.argv)
    config = None
    CURRENT = Path(__file__).resolve().parent
    log_path = Path.joinpath(CURRENT, "program.log")
    logging.basicConfig(
        level=logging.INFO,
        format="%(asctime)s [%(levelname)s] %(message)s",
        datefmt="%m/%d/%Y %I:%M:%S %p",
        handlers=[logging.FileHandler(str(log_path)), logging.StreamHandler()],
    )
    config_path = str(Path.joinpath(CURRENT, "config.json"))
    if not os.path.exists(config_path):
        logging.error("找不到配置文件")
        sys.exit()
    try:
        with open(config_path, encoding="utf-8") as f:
            config = json.load(f)
    except Exception as e:
        logging.exception("An exception occored.")
    else:
        window = MainWindow(config)
        window.show()
        app.exec()
