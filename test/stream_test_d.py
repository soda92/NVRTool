import os, sys
import pathlib

if not os.path.exists("serial_lib.pyd"):
    bin_path = pathlib.Path(__file__).resolve().parent.parent.joinpath("bin")
    os.add_dll_directory(str(bin_path))
    sys.path.insert(0, str(bin_path))

import sys
from PyQt6.QtCore import QSize
from PyQt6.QtWidgets import (
    QApplication,
    QMainWindow,
    QPushButton,
    QVBoxLayout,
    QWidget,
    QFrame,
)
from PyQt6.QtGui import QPalette, QColor

import stream_lib


class Color(QWidget):
    def __init__(self, color):
        super(Color, self).__init__()
        self.setAutoFillBackground(True)

        palette = self.palette()
        palette.setColor(QPalette.ColorRole.Window, QColor(color))
        self.setPalette(palette)


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("视频流测试")
        self.layout = QVBoxLayout()

        self.frame = QFrame()
        self.layout.addWidget(self.frame)

        self.button = QPushButton("stream")
        self.layout.addWidget(self.button)

        self.button.clicked.connect(self.stream)

        self.setMinimumSize(QSize(800, 600))

        self.widget = QWidget()
        self.widget.setLayout(self.layout)
        self.setCentralWidget(self.widget)

    def stream(self):
        hwnd = int(self.frame.winId())
        self.client = stream_lib.Client(hwnd, "192.168.104.70")
        self.client.stream()


if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    app.exec()
