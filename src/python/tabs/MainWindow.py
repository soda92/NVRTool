from PyQt5 import QtWidgets, QtCore, QtGui
from .video.Color import _translate


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
        from .MainTab import MainTab

        self.tab_page = MainTab()
        self.main_grid.addWidget(self.tab_page, 0, 0, 1, 1)

        from .stop_button import stop_button

        self.stop_button = stop_button()
        self.stop_button.setParent(self.centralwidget)

        self.tab_page.raise_()
        self.stop_button.raise_()

        self.setCentralWidget(self.centralwidget)

    def status(self):
        print()
