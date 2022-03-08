from PyQt5 import QtWidgets

class DeviceTab(QtWidgets.QWidget):
    def __init__(self):
        super(DeviceTab, self).__init__()
        self.setStyleSheet(
            """
            QLabel{
                border:1px solid grey;
                font-size: 22px;
                font-family: \"Microsoft Yahei\";
            }
        """
        )