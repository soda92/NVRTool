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


class VideoFrame(QtWidgets.QWidget):
    def __init__(self, name, ip) -> None:
        super(VideoFrame, self).__init__()
        layout = QtWidgets.QVBoxLayout()
        layout.setContentsMargins(0, 0, 0, 0)
        self.frame = QtWidgets.QFrame()
        layout.addWidget(self.frame)
        self.setLayout(layout)
        hwnd = int(self.frame.winId())
        self.name = name
        self.ip = ip
        self.client = stream_lib.Client(hwnd, ip)
        self.client.stream()

    def start_record(self, train_num, directory):
        args = f"ffmpeg -i rtsp://admin:hk123456@{self.ip}:554/Streaming/Channels/101 -c copy -map 0:v:0 -map 0:a:0 -segment_time 00:15:00 -f segment -strftime 1 {directory}%Y%m%d_%H%M%S_{train_num}_{self.name}.mp4"
        self.process = subprocess.Popen(args.split())

    def status(self):
        poll = self.process.poll()
        if poll is None:
            return True
        return False
