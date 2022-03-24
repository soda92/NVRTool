from PyQt6 import QtWidgets, QtCore, QtGui
from .MainTab import MainTab
from .stop_button import stop_button


class MainWindow(QtWidgets.QMainWindow):
    tab_page: MainTab
    stopbutton: stop_button

    def __init__(self, config):
        super(MainWindow, self).__init__()

        self.setWindowTitle(f"机车监控 - {config['name']}")
        self.setMinimumSize(QtCore.QSize(800, 600))

        self.centralwidget = QtWidgets.QWidget()
        self.centralwidget.setAutoFillBackground(True)

        p = self.centralwidget.palette()
        p.setColor(self.centralwidget.backgroundRole(), QtGui.QColor("black"))
        self.centralwidget.setPalette(p)

        self.main_grid = QtWidgets.QGridLayout()
        self.centralwidget.setLayout(self.main_grid)

        self.main_grid.setContentsMargins(0, 0, 0, 0)

        self.tab_page = MainTab(config)
        self.main_grid.addWidget(self.tab_page, 0, 0, 1, 1)

        self.setCentralWidget(self.centralwidget)
