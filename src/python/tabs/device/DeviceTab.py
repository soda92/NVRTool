from PyQt6 import QtWidgets


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

        
        self.button = QtWidgets.QPushButton()
        self.button.setParent(self)
        self.button.setText("系统设置")
        self.button.clicked.connect(self.showDialog)

    def showDialog(self):
        text, ok = QtWidgets.QInputDialog.getText(
            self, "输入密码", "输入密码", QtWidgets.QLineEdit.EchoMode.Password
        )
        if ok:
            if text == "administrator":
                self.exit()

    def exit(self):
        QtWidgets.QApplication.exit()
