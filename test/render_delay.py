import os
import sys

import pathlib
import datetime

this_path = pathlib.Path(__file__).resolve().parent
lib_path = this_path.joinpath("lib")
if not lib_path.exists():
    lib_path = this_path.parent.joinpath("lib")
    if not lib_path.exists():
        raise Exception("cannot find library path")

os.add_dll_directory(str(lib_path))
sys.path.insert(0, str(lib_path))


if os.environ.get("DEBUG") is not None:
    import stream_lib_mocked as stream_lib
else:
    import stream_lib

from PyQt6 import QtCore
from PyQt6 import QtWidgets
from PIL import ImageGrab
import imagehash

result_global: int = 0


class MainWindow(QtWidgets.QMainWindow):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("视频流测试")
        self.layout = QtWidgets.QVBoxLayout()

        self.frame = QtWidgets.QFrame()
        self.layout.addWidget(self.frame)

        self.button = QtWidgets.QPushButton("stream")
        self.layout.addWidget(self.button)

        self.button.clicked.connect(self.stream)

        self.setMinimumSize(QtCore.QSize(800, 600))

        self.widget = QtWidgets.QWidget()
        self.widget.setLayout(self.layout)
        self.setCentralWidget(self.widget)
        hwnd = int(self.frame.winId())
        self.client = stream_lib.Client(hwnd, "192.168.104.70")
        QtCore.QTimer.singleShot(500, self.init_screenshot)
        QtCore.QTimer.singleShot(1000, self.stream)
        self.count = 0

    def stream(self):
        self.client.stream()
        self.current = datetime.datetime.now()
        self.timer = QtCore.QTimer()
        self.timer.setInterval(100)
        self.timer.timeout.connect(self.check_screenshot)
        self.timer.start()

    def get_screenshot(self):
        pos0 = QtCore.QPoint(0, 0)
        pos1 = self.frame.size()
        # convert QSize to QPoint
        pos1 = QtCore.QPoint(pos1.width(), pos1.height())
        left, top = self.frame.mapToGlobal(pos0).x(), self.frame.mapToGlobal(pos0).y()
        right, bottom = (
            self.frame.mapToGlobal(pos1).x(),
            self.frame.mapToGlobal(pos1).y(),
        )
        left, top, right, bottom = list(
            map(lambda x: int(x * 1.25), [left, top, right, bottom])
        )
        # print(left, top, right, bottom)
        im = ImageGrab.grab(bbox=(left, top, right, bottom))
        return im

    def init_screenshot(self):
        self.im = self.get_screenshot()
        self.im.save("screenshot.png")

    def check_screenshot(self):
        im = self.get_screenshot()
        im.save("screenshot1.png")
        self.count += 1
        hash1 = imagehash.average_hash(self.im)
        hash2 = imagehash.average_hash(im)
        global result_global
        result_global = hash1 - hash2
        if result_global > 10:
            current = datetime.datetime.now()
            print(current - self.current)
            QtWidgets.QApplication.exit()


def test_delay():
    app = QtWidgets.QApplication(sys.argv)
    window = MainWindow()
    window.show()
    app.exec()


if __name__ == "__main__":
    test_delay()
