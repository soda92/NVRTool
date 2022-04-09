from PyQt6 import QtWidgets
from PyQt6 import QtCore

from .video.VideoTab import VideoTab
from .device.DeviceTab import DeviceTab
from .video.Color import _translate
from .sensor.SensorTab import SensorTab
from .event.EventsTab import EventsTab
from .stop_button import stop_button


class MainTab(QtWidgets.QTabWidget):
    tab_video: VideoTab
    tab_device: DeviceTab
    tab_sensor: SensorTab
    tab_events: EventsTab

    def __init__(self, config):
        super(MainTab, self).__init__()

        self.tab_video = VideoTab(config)
        self.addTab(self.tab_video, "")

        self.tab_device = DeviceTab(self.tab_video.video_box_tab.frames, config)
        self.addTab(self.tab_device, "")

        self.tab_sensor = SensorTab()
        self.addTab(self.tab_sensor, "")

        self.tab_events = EventsTab()
        self.addTab(self.tab_events, "")

        self.setTabText(self.indexOf(self.tab_video), _translate("MainWindow", "视频预览"))
        self.setTabText(self.indexOf(self.tab_device), _translate("MainWindow", "设备管理"))
        self.setTabText(self.indexOf(self.tab_sensor), _translate("MainWindow", "防火信息"))
        self.setTabText(self.indexOf(self.tab_events), _translate("MainWindow", "故障记录"))

        self.button = stop_button()
        self.setCornerWidget(self.button, QtCore.Qt.Corner.TopRightCorner)
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
