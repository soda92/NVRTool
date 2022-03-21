import subprocess
from typing import Any
from PyQt6 import QtWidgets
from PyQt6 import QtCore
import datetime
from typing import Tuple


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
    """A Video Frame Window."""

    double_click_signal = pyqtSignal(int)
    number: int = -1
    maxmized: bool = False
    group: str
    name: str
    detector: int
    address: str
    stream_addr: str
    count: int
    config: Any
    process: Any = None
    table_number: int = 0
    table_index: int = 0
    recording_signal = QtCore.pyqtSignal(int, int, bool)

    def __init__(
        self,
        group: str,
        name: str,
        detector: int,
        address: str,
        stream_addr: str,
        count: int,
        config: Any,
    ) -> None:
        super(VideoFrame, self).__init__()

        self.group = group
        self.name = name
        self.detector = detector
        self.address = address
        self.stream_addr = stream_addr
        self.count = count
        self.config = config

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

    def check_and_start_record(self):
        self.process_check = QtCore.QProcess(self)
        self.process_check.finished.connect(self.check)
        self.process_check.start(
            "ffprobe",
            [
                "-show_format",
                "-timeout",
                "1000000",
                "-loglevel",
                "quiet",
                self.stream_addr,
            ],
        )

    def check(self):
        output = self.process_check.readAllStandardOutput()
        output = str(output, "utf-8")
        # print(output)
        if output != "":
            self.start_record()
        else:
            self.timer = QtCore.QTimer()
            self.timer.setSingleShot(True)
            self.timer.setInterval(7000)
            self.timer.timeout.connect(self.check_and_start_record)
            self.timer.start()

    def start_record(self):
        current = datetime.datetime.now()
        formated_date = f"{current.year:04d}-{current.month:02d}-{current.day:02d}"
        date_folder = os.path.join(self.config["record_path_root"], formated_date)

        if not os.path.exists(date_folder):
            self.process = None
            return

        args = [
            "ffmpeg",
            "-i",
            self.stream_addr,
            "-c",
            "copy",
            "-map",
            "0:v:0",
            "-map",
            "0:a:0",
            "-f",
            "segment",
            "-strftime",
            "1",
            "-segment_time",
            "00:15:00",
            "-segment_atclocktime",
            "1",
            "-reset_timestamps",
            "1",
            "-segment_format",
            "mp4",
            f"{self.config['record_path_root']}/%Y-%m-%d/"
            + self.config["name"]
            + "_"
            + "北京蓝天多维"
            + "_"
            + f"{self.count:02d}"
            + "_"
            + f"{self.group}{self.name}"
            + "_"
            + "%Y%m%d_%H%M%S.mp4",
        ]
        self.process = QtCore.QProcess(
            self
        )  # Keep a reference to the QProcess (e.g. on self) while it's running.

        self.process.finished.connect(self.process_finished)  # Clean up once complete.
        self.process.start(args[0], args[1:])
        self.recording_signal.emit(self.table_number, self.table_index, True)

    def process_finished(self):
        print("Process finished.")
        self.process = None
        self.recording_signal.emit(self.table_number, self.table_index, False)
        self.timer = QtCore.QTimer()
        self.timer.setSingleShot(True)
        self.timer.setInterval(7000)
        self.timer.timeout.connect(self.check_and_start_record)
        self.timer.start()

    def status(self):
        if self.process is None:
            return False
        return True

    def status_text(self):
        if self.status():
            return "录像"
        else:
            return "未录像"

    def mouseDoubleClickEvent(self, event) -> None:
        self.double_click_signal.emit(self.number)
