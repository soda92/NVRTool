from typing import Any, List
from PyQt6 import QtWidgets
from PyQt6 import QtCore
from PyQt6 import QtGui
from ..video.VideoFrame import VideoFrame


class DeviceTab(QtWidgets.QWidget):
    """
    check record status regularly.

    start record if not.

    display record and disk status.
    """

    frames: List[VideoFrame] = None
    config: Any = None

    def __init__(self, frames: List[VideoFrame], config: Any):
        super(DeviceTab, self).__init__()
        self.setStyleSheet(
            """
            QLabel{
                border:1px solid grey;
                font-size: 22px;
                font-family: "Microsoft Yahei";
                color: white;
            }
        """
        )
        self.frames = frames
        self.config = config

        layout = QtWidgets.QVBoxLayout()
        label_record = QtWidgets.QLabel("录像状态")
        label_record.setAlignment(QtCore.Qt.AlignmentFlag.AlignHCenter)
        layout.addWidget(label_record)
        self.setLayout(layout)

        self.button = QtWidgets.QPushButton()
        # self.button.setParent(self)
        self.button.setText("系统设置")
        self.button.clicked.connect(self.showDialog)

        self.table_area = QtWidgets.QWidget()
        self.table_area_layout = QtWidgets.QHBoxLayout()
        self.table_area.setLayout(self.table_area_layout)

        self.tables = [QtWidgets.QTableWidget() for _ in config["groups"]]
        self.groups = [
            config["groups"][i]["group"] for i in range(len(config["groups"]))
        ]
        counts = [0 for _ in range(len(config["groups"]))]
        for i in range(len(self.frames)):
            for j in range(len(self.groups)):
                if self.frames[i].group == self.groups[j]:
                    counts[j] += 1

        for i in range(len(self.tables)):
            self.tables[i].setRowCount(counts[i])
            self.tables[i].setColumnCount(2)
            self.tables[i].setHorizontalHeaderItem(0, QtWidgets.QTableWidgetItem("位置"))
            self.tables[i].setHorizontalHeaderItem(1, QtWidgets.QTableWidgetItem("状态"))
            self.tables[i].setStyleSheet(
                """
                QTableWidget{
                    margin-left: auto;
                    margin-right: auto;
                    border: none;
                }
                QTableWidget::item{
                    padding-top: 5px;
                    padding-bottom: 5px;
                }
                """
            )
            self.tables[i].setSizePolicy(
                QtWidgets.QSizePolicy.Policy.Maximum,
                QtWidgets.QSizePolicy.Policy.Expanding,
            )
            self.tables[i].setSizeAdjustPolicy(
                QtWidgets.QAbstractScrollArea.SizeAdjustPolicy.AdjustToContents
            )
            self.tables[i].horizontalHeader().setSectionResizeMode(
                0, QtWidgets.QHeaderView.ResizeMode.ResizeToContents
            )
            self.tables[i].horizontalHeader().setSectionResizeMode(
                1, QtWidgets.QHeaderView.ResizeMode.ResizeToContents
            )
            self.tables[i].verticalHeader().setSectionResizeMode(
                QtWidgets.QHeaderView.ResizeMode.Stretch
            )

            self.table_area_layout.addWidget(
                self.tables[i], QtCore.Qt.AlignmentFlag.AlignCenter
            )

        font = QtGui.QFont()
        font.setPixelSize(22)
        font.setFamily("Microsoft Yahei")
        counts = [0 for _ in range(len(config["groups"]))]
        for i in range(len(self.frames)):
            for j in range(len(self.groups)):
                if self.frames[i].group == self.groups[j]:
                    item0 = QtWidgets.QTableWidgetItem(
                        f"{self.frames[i].group}{self.frames[i].name}"
                    )
                    self.frames[i].table_number = j
                    self.frames[i].table_index = counts[j]
                    self.frames[i].recording_signal.connect(self.refresh)
                    item0.setFont(font)
                    item0.setTextAlignment(QtCore.Qt.AlignmentFlag.AlignCenter)
                    self.tables[j].setItem(
                        counts[j],
                        0,
                        item0,
                    )
                    item1 = QtWidgets.QTableWidgetItem(
                        f"{self.frames[i].status_text()}"
                    )
                    item1.setFont(font)
                    item1.setTextAlignment(QtCore.Qt.AlignmentFlag.AlignCenter)
                    self.tables[j].setItem(
                        counts[j],
                        1,
                        item1,
                    )
                    counts[j] += 1

        layout.addWidget(self.table_area)
        label_disk = QtWidgets.QLabel("硬盘状态")
        label_disk.setAlignment(QtCore.Qt.AlignmentFlag.AlignHCenter)
        layout.addWidget(label_disk)

        self.disk_status = QtWidgets.QTableWidget()
        self.disk_status.setRowCount(2)
        self.disk_status.setColumnCount(3)
        self.disk_status.setSizeAdjustPolicy(
            QtWidgets.QAbstractScrollArea.SizeAdjustPolicy.AdjustToContents
        )
        self.disk_status.setSizePolicy(
            QtWidgets.QSizePolicy.Policy.Maximum, QtWidgets.QSizePolicy.Policy.Minimum
        )
        self.disk_status.setObjectName("disk")
        self.disk_status.setStyleSheet("border: none;")

        self.disk_status.setHorizontalHeaderItem(0, QtWidgets.QTableWidgetItem("位置"))
        self.disk_status.setHorizontalHeaderItem(1, QtWidgets.QTableWidgetItem("总容量"))
        self.disk_status.setHorizontalHeaderItem(2, QtWidgets.QTableWidgetItem("剩余"))
        self.disk_status.horizontalHeader().setSectionResizeMode(
            0, QtWidgets.QHeaderView.ResizeMode.ResizeToContents
        )
        self.disk_status.horizontalHeader().setSectionResizeMode(
            1, QtWidgets.QHeaderView.ResizeMode.ResizeToContents
        )
        self.disk_status.horizontalHeader().setSectionResizeMode(
            2, QtWidgets.QHeaderView.ResizeMode.ResizeToContents
        )
        self.disk_status.verticalHeader().setSectionResizeMode(
            QtWidgets.QHeaderView.ResizeMode.ResizeToContents
        )

        self.disk_status_w = QtWidgets.QWidget()
        self.disk_status_l = QtWidgets.QHBoxLayout()
        self.disk_status_l.addWidget(
            self.disk_status, QtCore.Qt.AlignmentFlag.AlignHCenter
        )
        self.disk_status_w.setLayout(self.disk_status_l)

        layout.addWidget(self.disk_status_w)

        layout.addWidget(self.button)
        self.setObjectName("devicetab")

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
        self.start_record()

    def start_record(self):
        for frame in self.frames:
            frame.check_and_start_record()
        # self.frames[0].check_and_start_record()


    @QtCore.pyqtSlot(int, int, bool)
    def refresh(self, table_number: int, table_index: int, status: bool) -> None:
        item = self.tables[table_number].item(table_index, 1)
        if status:
            item.setText("录像")
            item.setForeground(QtGui.QColor("green"))
        else:
            item.setText("未录像")
            item.setForeground(QtGui.QColor("red"))


    def showDialog(self) -> None:
        """Exit dialog. check password and exit."""

        text, ok = QtWidgets.QInputDialog.getText(
            self, "输入密码", "输入密码", QtWidgets.QLineEdit.EchoMode.Password
        )
        if ok:
            if text == "administrator":
                self.exit()

    def exit(self):
        """Exit the program for maintenance."""
        QtWidgets.QApplication.exit()
