import os, sys
import pathlib

if not os.path.exists("serial_lib.pyd"):
    bin_path = pathlib.Path(__file__).resolve().parent.parent.joinpath("bin")
    os.add_dll_directory(str(bin_path))
    sys.path.insert(0, str(bin_path))


from PyQt5 import QtWidgets
from PyQt5 import QtCore
from PyQt5 import QtGui

_translate = QtCore.QCoreApplication.translate


class Color(QtWidgets.QWidget):
    def __init__(self, color):
        super(Color, self).__init__()
        self.setAutoFillBackground(True)

        palette = self.palette()
        palette.setColor(QtGui.QPalette.ColorRole.Window, QtGui.QColor(color))
        self.setPalette(palette)


import os

os.add_dll_directory("D:\\boost_1_78_0\\stage\\lib")
os.add_dll_directory("D:\\HCNetSDK\\lib")
import stream_lib


class VideoGrid(QtWidgets.QGridLayout):
    def __init__(self, max_count) -> None:
        super(VideoGrid, self).__init__()
        self.max_count = max_count
        # self.count = 0
        self.current_row = 0
        self.current_column = 0

    def addWidget(self, widget):
        # print(self.current_row,self.current_column)
        super().addWidget(widget, self.current_row, self.current_column)
        self.current_column += 1
        if self.current_column >= self.max_count:
            self.current_row += 1
            self.current_column = 0


import subprocess


class VideoFrame(QtWidgets.QWidget):
    def __init__(self, name, ip) -> None:
        super(VideoFrame, self).__init__()
        layout = QtWidgets.QVBoxLayout()
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


class MainWindow(QtWidgets.QMainWindow):
    def __init__(self):
        super(MainWindow, self).__init__()

        self.setWindowTitle("太原机车防火视频程序")
        self.setMinimumSize(QtCore.QSize(800, 600))

        self.centralwidget = QtWidgets.QWidget()
        self.centralwidget.setAutoFillBackground(True)
        p = self.centralwidget.palette()
        p.setColor(self.centralwidget.backgroundRole(), QtGui.QColor("black"))
        self.centralwidget.setPalette(p)
        self.main_grid = QtWidgets.QGridLayout()
        self.centralwidget.setLayout(self.main_grid)
        self.main_grid.setContentsMargins(0, 0, 0, 0)

        self.tab_page = QtWidgets.QTabWidget()
        self.main_grid.addWidget(self.tab_page, 0, 0, 1, 1)
        self.tab_page.setStyleSheet(
            """
            QTabBar::tab  {
                background: black;
                color: white;
                height: 50px;
                border: 1px solid white;
                width: 150px;
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
        self.tab_video = QtWidgets.QWidget()
        self.tab_video.setStyleSheet(
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
        self.tab_video.setLayout(self.video_box_grid)
        self.tab_page.addTab(self.tab_video, "")
        self.tab_page.setTabText(
            self.tab_page.indexOf(self.tab_video), _translate("MainWindow", "视频预览")
        )

        self.tab_record = QtWidgets.QWidget()
        self.tab_record.setStyleSheet(
            """
            QLabel{
                border:1px solid grey;
                font-size: 22px;
                font-family: \"Microsoft Yahei\";
            }
        """
        )
        self.tab_page.addTab(self.tab_record, "")
        self.tab_page.setTabText(
            self.tab_page.indexOf(self.tab_record), _translate("MainWindow", "设备管理")
        )

        self.tab_fire = QtWidgets.QWidget()
        self.tab_fire.setStyleSheet(
            """
            QLabel{
                border:1px solid grey;
                font-size: 22px;
                font-family: \"Microsoft Yahei\";
            }
        """
        )
        self.tab_fire.setObjectName("tab_record")
        self.tab_page.addTab(self.tab_fire, "")
        self.tab_page.setTabText(
            self.tab_page.indexOf(self.tab_fire), _translate("MainWindow", "火警信息")
        )

        self.tab_log = QtWidgets.QWidget()
        self.tab_log.setStyleSheet(
            """
            QLabel{
                border:1px solid grey;
                font-size: 22px;
                font-family: \"Microsoft Yahei\";
            }
        """
        )
        self.tab_log.setObjectName("tab_record")
        self.tab_page.addTab(self.tab_log, "")
        self.tab_page.setTabText(
            self.tab_page.indexOf(self.tab_log), _translate("MainWindow", "事件记录")
        )

        self.stop_warn_button = QtWidgets.QPushButton(self.centralwidget)
        self.stop_warn_button.setGeometry(QtCore.QRect(692, 5, 101, 41))
        self.stop_warn_button.setStyleSheet(
            """
            QPushButton#stop_warn_button{
                color: white;
                background-color: black;
                border: 1px solid white;
                font-size: 22px;
                font-family: "Microsoft Yahei";
            }
            QPushButton#stop_warn_button::pressed{
                background-color: red;
                color: white;
            }
        """
        )
        self.stop_warn_button.setObjectName("stop_warn_button")
        self.stop_warn_button.setText(_translate("MainWindow", "停止报警"))

        self.stop_warn_button.clicked.connect(self.print_c1)
        self.tab_page.raise_()
        self.stop_warn_button.raise_()

        self.setCentralWidget(self.centralwidget)

    def print_c1(self):
        print("stop warn button clicked")

    def status(self):
        print()


if __name__ == "__main__":
    import sys

    app = QtWidgets.QApplication(sys.argv)

    window = MainWindow()
    window.show()
    app.exec()
