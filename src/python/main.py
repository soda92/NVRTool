from tabs.MainWindow import MainWindow
from PyQt6 import QtWidgets
import sys
import json
from pathlib import Path

CURRENT = Path(__file__).resolve().parent


if __name__ == "__main__":
    app = QtWidgets.QApplication(sys.argv)
    config = None
    config_path = Path.joinpath(CURRENT, "config.json")
    with open(str(config_path), encoding="utf-8") as f:
        config = json.load(f)
    window = MainWindow(config)
    window.show()
    app.exec()
