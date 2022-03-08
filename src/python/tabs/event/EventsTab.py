from PyQt6 import QtWidgets

class EventsTab(QtWidgets.QWidget):
    def __init__(self):
        super(EventsTab, self).__init__()
        self.setStyleSheet(
            """
            QLabel{
                border:1px solid grey;
                font-size: 22px;
                font-family: \"Microsoft Yahei\";
            }
        """
        )
