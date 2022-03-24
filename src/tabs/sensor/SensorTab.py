from PyQt6 import QtWidgets
from PyQt6 import QtCore


class SensorTab(QtWidgets.QWidget):
    def __init__(self):
        super(SensorTab, self).__init__()
        self.setStyleSheet(
            """
            QLabel{
                border:1px solid grey;
                font-size: 22px;
                font-family: \"Microsoft Yahei\";
            }
        """
        )
        self.setObjectName("tab_sensor")

        self.sensor_status = QtWidgets.QTableWidget()
        self.sensor_status.setRowCount(4)
        self.sensor_status.setColumnCount(4)
        self.sensor_status.setSizeAdjustPolicy(
            QtWidgets.QAbstractScrollArea.SizeAdjustPolicy.AdjustToContents
        )
        self.sensor_status.setSizePolicy(
            QtWidgets.QSizePolicy.Policy.Maximum, QtWidgets.QSizePolicy.Policy.Minimum
        )
        self.sensor_status.setObjectName("sensor")
        self.sensor_status.setStyleSheet("border: none;")

        self.sensor_status.setHorizontalHeaderItem(0, QtWidgets.QTableWidgetItem("位置"))
        self.sensor_status.setHorizontalHeaderItem(1, QtWidgets.QTableWidgetItem("类型"))
        self.sensor_status.setHorizontalHeaderItem(2, QtWidgets.QTableWidgetItem("状态"))
        self.sensor_status.setHorizontalHeaderItem(3, QtWidgets.QTableWidgetItem("供应商"))
        self.sensor_status.horizontalHeader().setSectionResizeMode(
            0, QtWidgets.QHeaderView.ResizeMode.Stretch
        )
        self.sensor_status.horizontalHeader().setSectionResizeMode(
            1, QtWidgets.QHeaderView.ResizeMode.Stretch
        )
        self.sensor_status.horizontalHeader().setSectionResizeMode(
            2, QtWidgets.QHeaderView.ResizeMode.Stretch
        )
        self.sensor_status.verticalHeader().setSectionResizeMode(
            QtWidgets.QHeaderView.ResizeMode.ResizeToContents
        )

        # self.disk_status_w = QtWidgets.QWidget()
        self.sensor_status_l = QtWidgets.QHBoxLayout()
        self.sensor_status_l.addWidget(
            self.sensor_status, QtCore.Qt.AlignmentFlag.AlignHCenter
        )
        # self.disk_status_w.setLayout(self.disk_status_l)
        self.setLayout(self.sensor_status_l)

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
                margin: 40px;
            }

            QTableWidget QTableCornerButton::section {
                background-color: #646464;
                border: none;
            }

            """
        )
