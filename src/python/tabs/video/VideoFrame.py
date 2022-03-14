import subprocess
from PyQt6 import QtWidgets


import os, sys
import pathlib

if not os.path.exists("serial_lib.pyd"):
    this_path = pathlib.Path(__file__).resolve().parent
    bin_path = this_path.parent.parent.parent.parent.joinpath("bin")
    os.add_dll_directory(str(bin_path))
    sys.path.insert(0, str(bin_path))

import stream_lib

from PyQt6.QtCore import pyqtSignal


class VideoFrame(QtWidgets.QWidget):
    double_click_signal = pyqtSignal(int)
    number: int = -1
    maxmized: bool = False
    group: str
    name: str
    detector: int
    address: str
    stream_addr: str

    def __init__(
        self, group: str, name: str, detector: int, address: str, stream_addr: str
    ) -> None:
        super(VideoFrame, self).__init__()

        self.group = group
        self.name = name
        self.detector = detector
        self.address = address
        self.stream_addr = stream_addr

        layout = QtWidgets.QVBoxLayout()
        layout.setContentsMargins(0, 0, 0, 0)
        self.setStyleSheet("border: 1px solid white;")
        self.frame = QtWidgets.QFrame()
        self.text = QtWidgets.QLabel()
        self.text.setParent(self)
        self.text.setGeometry(0, 0, 0, 0)
        self.text.raise_()
        # text.hide()

        self.text.setStyleSheet("color: white; border: none;")
        layout.addWidget(self.frame)
        self.setLayout(layout)
        hwnd = int(self.frame.winId())

        self.text.setText(f"无信号\n{self.group}{self.name}\n{self.address}")
        self.client = stream_lib.Client(hwnd, address)
        self.client.stream()

    def start_record(self, train_num, directory):
        args = f"ffmpeg -i rtsp://admin:hk123456@{self.ip}:554/Streaming/Channels/101 -c copy -map 0:v:0 -map 0:a:0 -segment_time 00:15:00 -f segment -strftime 1 {directory}/%Y%m%d/_%H%M%S_{train_num}_{self.name}.mp4"
        self.process = subprocess.Popen(args.split())

    def status(self):
        poll = self.process.poll()
        if poll is None:
            return True
        return False

    def mouseDoubleClickEvent(self, event) -> None:
        self.double_click_signal.emit(self.number)
