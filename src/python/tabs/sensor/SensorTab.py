from PyQt5 import QtWidgets

class SensorTab(QtWidgets.QWidget):
    def __init__(self):
        super(SensorTab, self).__init__()
        self.setStyleSheet(
            """
            QLabel{
                border:1px solid grey;
                font-size: 22px;
                font-family: \"Microsoft Yahei\";
            }
        """
        )
        self.setObjectName("tab_sensor")
