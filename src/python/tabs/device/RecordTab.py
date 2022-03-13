from PyQt6 import QtWidgets

class RecordStatus(QtWidgets.QTableWidget):
    def __init__(self):
        super(RecordStatus, self).__init__()
        self.setColumnCount()


class RecordTab(QtWidgets.QTabWidget):
    def __init__(self):
        super(RecordTab, self).__init__()

        layout = QtWidgets.QTabWidget()
