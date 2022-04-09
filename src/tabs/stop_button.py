from PyQt6 import QtWidgets, QtCore

_translate = QtCore.QCoreApplication.translate


class stop_button(QtWidgets.QPushButton):
    """停止报警按钮"""
    def __init__(self):
        super(stop_button, self).__init__()
        self.setStyleSheet(
            """
            QPushButton#stop_warn_button{
                color: white;
                background-color: black;
                border: 1px solid white;
                font-size: 22px;
                font-family: "Microsoft Yahei";
                margin: 5px;
                padding: 3px;
            }
            QPushButton#stop_warn_button::pressed{
                background-color: red;
                color: white;
            }
        """
        )

        self.setObjectName("stop_warn_button")
        self.setText(_translate("MainWindow", "停止报警"))

        self.clicked.connect(self.print_c1)

    def print_c1(self):
        """TODO 按下事件"""
        print("stop warn button clicked")
