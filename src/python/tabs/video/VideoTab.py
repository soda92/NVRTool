from PyQt6 import QtWidgets
from .VideoGrid import VideoGrid
from .Color import Color
from .VideoFrame import VideoFrame
from .Color import _translate


class VideoTab(QtWidgets.QWidget):
    def __init__(self):
        super(VideoTab, self).__init__()
        self.setStyleSheet(
            """
            *{
                font-size: 22px;
                font-family: \"Microsoft Yahei\";
            }
            
            QPushButton{
                border: 1px solid grey;
                height: 30px;
            }
        """
        )
        self.video_box_grid = QtWidgets.QVBoxLayout()
        self.video_area_main = QtWidgets.QWidget()
        self.video_grid = VideoGrid(3)
        self.video_grid.setContentsMargins(0, 0, 0, 0)
        self.video_area_main.setLayout(self.video_grid)
        self.video_area_main.setMinimumHeight(int(self.video_area_main.width() * 3 / 4))
        self.video_box_grid.addWidget(self.video_area_main)
        self.video_box_grid.addWidget(Color("blue"))
        self.setLayout(self.video_box_grid)

        addrs1 = [
            "192.168.104.70",
            "192.168.104.71",
            "192.168.104.72",
            "192.168.104.73",
            "192.168.104.74",
            "192.168.104.75",
        ]
        addrs2 = [
            "192.168.104.80",
            "192.168.104.81",
            "192.168.104.82",
            "192.168.104.83",
            "192.168.104.84",
            "192.168.104.85",
        ]
        self.video_frames1 = [VideoFrame("Local", addr) for addr in addrs1]
        # self.v.start_record("Test", "D:/")
        for frame in self.video_frames1:
            self.video_grid.addWidget(frame)
