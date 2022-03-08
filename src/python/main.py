from tabs.MainWindow import MainWindow
from PyQt5 import QtWidgets


if __name__ == "__main__":
    import sys

    app = QtWidgets.QApplication(sys.argv)

    window = MainWindow()
    window.show()
    app.exec()
