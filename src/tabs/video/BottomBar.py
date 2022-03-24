from PyQt6 import QtWidgets


class BottomBar(QtWidgets.QWidget):
    button1 = QtWidgets.QPushButton()
    button2 = QtWidgets.QPushButton()

    def __init__(self):
        super(BottomBar, self).__init__()
        layout = QtWidgets.QHBoxLayout()
        self.button1.setStyleSheet("color: white;")
        self.button2.setStyleSheet("color: white;")
        self.button1.setText("A节")
        self.button2.setText("B节")
        layout.addWidget(self.button1)
        layout.addWidget(self.button2)
        self.setLayout(layout)
