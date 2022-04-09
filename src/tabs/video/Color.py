from PyQt6 import QtWidgets, QtCore, QtGui

_translate = QtCore.QCoreApplication.translate
from PyQt6 import QtCore


class Color(QtWidgets.QWidget):
    double_click_signal = QtCore.pyqtSignal(int)
    number: int = -1
    maxmized: bool = False
    group: str
    name: str
    detector: int
    address: str
    stream_addr: str

    def __init__(self, color):
        super(Color, self).__init__()
        self.setAutoFillBackground(True)

        palette = self.palette()
        palette.setColor(QtGui.QPalette.ColorRole.Window, QtGui.QColor(color))
        self.setPalette(palette)
