from PyQt6 import QtWidgets
from .VideoGrid import VideoGrid
from .VideoFrame import VideoFrame
from typing import List
from .Color import Color


class VideoBoxTab(QtWidgets.QTabWidget):
    frames: List[VideoFrame] = []
    grids: List[VideoGrid] = []
    tabs: List[QtWidgets.QWidget] = []

    def __init__(self, config):
        super(VideoBoxTab, self).__init__()
        if config["window_size"] != 4:
            self.grids = [VideoGrid(3) for _ in range(2)]
        else:
            self.grids = [VideoGrid(2) for _ in range(4)]

        self.tabs = [QtWidgets.QWidget() for _ in self.grids]
        for i in range(len(self.tabs)):
            self.tabs[i].setLayout(self.grids[i])
            self.addTab(self.tabs[i], "")

        for group in config["groups"]:
            for device in group["devices"]:
                self.frames.append(
                    VideoFrame(
                        group=group["group"],
                        name=device["name"],
                        detector=device["detector"],
                        address=device["address"],
                        stream_addr=device["stream_addr"],
                    )
                )

        if config["window_size"] == 4:
            for i in range(len(config["groups"][0]["devices"])):
                if i < 4:
                    self.grids[0].addWidget(self.frames[i])
                else:
                    self.grids[1].addWidget(self.frames[i])
            for _ in range(len(self.grids[1].widgets), 3):
                self.grids[1].addWidget(Color("black"))
            for i in range(len(config["groups"][1]["devices"])):
                if i < 4:
                    self.grids[2].addWidget(
                        self.frames[len(config["groups"][0]["devices"]) + i]
                    )
                else:
                    self.grids[3].addWidget(
                        self.frames[len(config["groups"][0]["devices"]) + i]
                    )
            for _ in range(len(self.grids[3].widgets), 3):
                self.grids[3].addWidget(Color("black"))
            self.setTabText(0, f"{self.grids[0].widgets[0].group}-第一页")
            self.setTabText(1, f"{self.grids[1].widgets[0].group}-第二页")
            self.setTabText(2, f"{self.grids[2].widgets[0].group}-第一页")
            self.setTabText(3, f"{self.grids[3].widgets[0].group}-第二页")

        elif config["window_size"] == 9:
            for i in range(9):
                self.grids[0].addWidget(self.frames[i])
            for i in range(9, len(self.frames)):
                self.grids[1].addWidget(self.frames[i])

            for i in range(len(self.grids[1].widgets), 9):
                self.grids[1].addWidget(Color("black"))

            self.setTabText(0, "第一页")
            self.setTabText(1, "第二页")
        else:
            for i in range(len(config["groups"][0]["devices"])):
                self.grids[0].addWidget(self.frames[i])
            for i in range(len(config["groups"][0]["devices"]), len(self.frames)):
                self.grids[1].addWidget(self.frames[i])
            self.setTabText(0, self.grids[0].widgets[0].group)
            self.setTabText(1, self.grids[1].widgets[0].group)

        self.setStyleSheet(
            """
            QTabBar::tab  {
                background: black;
                color: white;
                height: 50px;
                border: 1px solid white;
                width: 130px;
                font-size: 22px;
                font-family: \"Microsoft Yahei\";
            }

            QTabBar{
                width: 100%;
                border-bottom: solid 1px white;
                background-color: black;
                color: white;
            }

            QTabBar::tab::selected  {
                background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:1 #000, stop:0  #3cb43a);
                color: white;
                border-bottom: black;
            }

            QTabWidget::pane{
                border: 1px solid black;
            }

            QTabWidget{
                outline: 5px solid red;
            }

            QTabWidget::pane {
                border: 1px solid white;
                top:-1px; 
                background:  black;
            } 

            QTabBar::tab {
                background: black; 
                border: 1px solid white; 
                /*padding: 15px;*/
            } 

            QTabBar::tab:selected { 
                background: qlineargradient(spread:pad, x1:0, y1:0, x2:0, y2:1, stop:1 #000, stop:0  #3cb43a); 
                margin-bottom: -1px;
            }
        """
        )


class VideoTab(QtWidgets.QWidget):
    video_box_tab: VideoBoxTab

    def __init__(self, config):
        super(VideoTab, self).__init__()

        self.layout = QtWidgets.QVBoxLayout()
        self.video_box_tab = VideoBoxTab(config)
        self.layout.addWidget(self.video_box_tab)
        self.setLayout(self.layout)

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
