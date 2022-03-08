from PyQt6 import QtWidgets
from .video.Color import _translate


class MainTab(QtWidgets.QTabWidget):
    def __init__(self):
        super(MainTab, self).__init__()
        self.setStyleSheet(
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

        from .video.VideoTab import VideoTab

        self.tab_video = VideoTab()
        self.addTab(self.tab_video, "")

        self.setTabText(self.indexOf(self.tab_video), _translate("MainWindow", "视频预览"))

        from .device.DeviceTab import DeviceTab

        self.tab_device = DeviceTab()
        self.addTab(self.tab_device, "")

        self.setTabText(self.indexOf(self.tab_device), _translate("MainWindow", "设备管理"))

        from .sensor.SensorTab import SensorTab

        self.tab_sensor = SensorTab()
        self.addTab(self.tab_sensor, "")
        self.setTabText(self.indexOf(self.tab_sensor), _translate("MainWindow", "火警信息"))

        from .event.EventsTab import EventsTab

        self.tab_events = EventsTab()
        self.addTab(self.tab_events, "")
        self.setTabText(self.indexOf(self.tab_events), _translate("MainWindow", "事件记录"))
