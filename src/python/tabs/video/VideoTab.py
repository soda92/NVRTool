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
        self.video_area = Color("red")
        self.video_grid.addWidget(self.video_area)
        self.video_grid.addWidget(Color("green"))
        self.video_grid.addWidget(Color("green"))
        self.video_grid.addWidget(Color("red"))
        self.v = VideoFrame("Local", "192.168.104.70")
        # self.v.start_record("Test", "D:/")
        self.video_grid.addWidget(self.v)
        self.video_box_grid.addWidget(self.video_area_main)
        self.video_box_grid.addWidget(Color("blue"))
        self.setLayout(self.video_box_grid)
