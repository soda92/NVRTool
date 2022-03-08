from PyQt5 import QtWidgets, QtCore, QtGui

_translate = QtCore.QCoreApplication.translate


class Color(QtWidgets.QWidget):
    def __init__(self, color):
        super(Color, self).__init__()
        self.setAutoFillBackground(True)

        palette = self.palette()
        palette.setColor(QtGui.QPalette.ColorRole.Window, QtGui.QColor(color))
        self.setPalette(palette)
