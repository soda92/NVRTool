from PyQt6 import QtWidgets
from PyQt6 import QtCore


class EventsTab(QtWidgets.QWidget):
    "记录故障信息"
    def __init__(self):
        super(EventsTab, self).__init__()
        self.setStyleSheet(
            """
            QLabel{
                border:1px solid grey;
                font-size: 22px;
                font-family: "Microsoft Yahei";
            }
        """
        )

        self.events_table = QtWidgets.QTableWidget()
        self.events_table.setColumnCount(2)
        self.events_table.setRowCount(10)
        self.events_table.setSizeAdjustPolicy(
            QtWidgets.QAbstractScrollArea.SizeAdjustPolicy.AdjustToContents
        )
        self.events_table.setSizePolicy(
            QtWidgets.QSizePolicy.Policy.Expanding, QtWidgets.QSizePolicy.Policy.Minimum
        )
        self.events_table.setObjectName("sensor")
        self.events_table.setStyleSheet("border: none;")

        self.events_table.setHorizontalHeaderItem(0, QtWidgets.QTableWidgetItem("故障信息"))
        self.events_table.setHorizontalHeaderItem(1, QtWidgets.QTableWidgetItem("时间"))
        self.events_table.horizontalHeader().setSectionResizeMode(
            0, QtWidgets.QHeaderView.ResizeMode.Stretch
        )
        self.events_table.horizontalHeader().setSectionResizeMode(
            1, QtWidgets.QHeaderView.ResizeMode.ResizeToContents
        )
        self.events_table.verticalHeader().setSectionResizeMode(
            QtWidgets.QHeaderView.ResizeMode.ResizeToContents
        )
        self.total_l = QtWidgets.QVBoxLayout()
        self.events_table_w = QtWidgets.QWidget()
        self.events_table_l = QtWidgets.QHBoxLayout()
        self.events_table_w.setLayout(self.events_table_l)
        self.events_table_l.addWidget(
            self.events_table, QtCore.Qt.AlignmentFlag.AlignHCenter
        )
        self.previous = QtWidgets.QPushButton("上一页")
        self.next = QtWidgets.QPushButton("下一页")
        self.clear = QtWidgets.QPushButton("清除")
        self.button_l = QtWidgets.QHBoxLayout()
        self.button_l.addWidget(self.previous)
        self.button_l.addWidget(self.next)
        self.button_l.addWidget(self.clear)
        self.button_w = QtWidgets.QWidget()
        self.button_w.setStyleSheet(
            """
            QPushButton{
                font-size: 22px;
                border: 1px solid white;
            }
            """
        )
        self.button_w.setLayout(self.button_l)

        self.total_l.addWidget(self.events_table_w)
        self.total_l.addWidget(self.button_w)
        self.setLayout(self.total_l)

        self.setStyleSheet(
            """
            *{
                color: white;
                background-color: black;
            }

            QLabel{
                font-size: 22px;
            }

            QHeaderView::section {
                background-color: #646464;
                padding: 4px;
                border: 0px;
                font-size: 14pt;
            }

            QTableWidget {
                gridline-color: #fffff8;
                font-size: 12pt;
            }

            QTableWidget QTableCornerButton::section {
                background-color: #646464;
                border: none;
            }

            """
        )
