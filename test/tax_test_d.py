import os, sys
import pathlib

if not os.path.exists("serial_lib.pyd"):
    bin_path = pathlib.Path(__file__).resolve().parent.parent.joinpath("bin")
    os.add_dll_directory(str(bin_path))
    sys.path.insert(0, str(bin_path))


import json
import os
from PyQt6 import QtWidgets
from PyQt6 import QtCore
from PyQt6 import QtGui


class Color(QtWidgets.QWidget):
    def __init__(self, color):
        super(Color, self).__init__()
        self.setAutoFillBackground(True)
        self.setMinimumSize(100, 100)
        palette = self.palette()
        palette.setColor(QtGui.QPalette.ColorRole.Window, QtGui.QColor(color))
        self.setPalette(palette)


class Status(QtWidgets.QWidget):
    def __init__(self, text):
        super(Status, self).__init__()
        self.layout = QtWidgets.QHBoxLayout()
        self.setLayout(self.layout)
        self.label = QtWidgets.QLabel(text + ": ")
        self.label.setMinimumWidth(80)
        self.layout.addWidget(self.label)
        self.value = QtWidgets.QLabel()
        self.value.setStyleSheet(
            """
            QLabel{
                border: 1px dotted gray;
                qproperty-alignment: AlignCenter;
            }
        """
        )
        self.value.setMinimumWidth(150)
        # self.value.setDisabled(True)
        self.layout.addWidget(self.value)

    def set_val(self, val):
        self.value.setText(str(val))


import typing


class MyGrid(QtWidgets.QGridLayout):
    def __init__(self, max_count) -> None:
        super(MyGrid, self).__init__()
        self.max_count = max_count
        # self.count = 0
        self.current_row = 0
        self.current_column = 0

    def addWidget(self, widget):
        # print(self.current_row,self.current_column)
        super().addWidget(widget, self.current_row, self.current_column)
        self.current_column += 1
        if self.current_column >= self.max_count:
            self.current_row += 1
            self.current_column = 0


class MainWindow(QtWidgets.QMainWindow):
    def __init__(self):
        super(MainWindow, self).__init__()

        # self.setMinimumSize(QtCore.QSize(800, 600))
        self.setWindowTitle("TAX模拟客户端")
        self.widget = QtWidgets.QWidget()
        self.setCentralWidget(self.widget)
        self.layout = MyGrid(3)
        self.widget.setLayout(self.layout)
        # self.layout.addWidget(Color("red"))
        self.TaxTime = Status("系统时间")
        self.FactRoute = Status("实际交路号")
        self.ValvePressure = Status("列车管压")
        self.DieselSpeedAndOriginalCurrent = Status("柴转/原边电流")
        self.Speed = Status("当前速度")
        self.MaxSpeed = Status("限速")
        self.TrainSign = Status("机车信号")
        self.TrainState = Status("机车工况")
        self.SignNo = Status("信号机编号")
        self.SignType = Status("信号机种类")
        self.Signpost = Status("当前公里标")
        self.CarWeight = Status("总重")
        self.CarLong = Status("计长")
        self.CarCount = Status("辆数")
        self.TrainFlag = Status("信号机种类")
        self.TrainNum = Status("车次")
        self.SectionNo = Status("区段号/交路号")
        self.StationNo = Status("车站号")
        self.DriverNo = Status("司机号")
        self.CopilotNo = Status("副司机号")
        self.EngineNo = Status("机车号")
        self.EngineType = Status("机车型号")
        self.DevicePlace = Status("设备位置")
        self.PipePressure = Status("管压")
        self.TrainTypeId = Status("车次字符")
        self.layout.addWidget(self.TaxTime)
        self.layout.addWidget(self.FactRoute)
        self.layout.addWidget(self.DieselSpeedAndOriginalCurrent)
        self.layout.addWidget(self.Speed)
        self.layout.addWidget(self.TrainSign)
        self.layout.addWidget(self.TrainState)
        self.layout.addWidget(self.SignNo)
        self.layout.addWidget(self.SignType)
        self.layout.addWidget(self.Signpost)
        self.layout.addWidget(self.CarWeight)
        self.layout.addWidget(self.CarLong)
        self.layout.addWidget(self.CarCount)
        self.layout.addWidget(self.TrainFlag)
        self.layout.addWidget(self.TrainNum)
        self.layout.addWidget(self.SectionNo)
        self.layout.addWidget(self.StationNo)
        self.layout.addWidget(self.DriverNo)
        self.layout.addWidget(self.CopilotNo)
        self.layout.addWidget(self.EngineNo)
        self.layout.addWidget(self.EngineType)
        self.layout.addWidget(self.DevicePlace)
        self.layout.addWidget(self.PipePressure)
        self.layout.addWidget(self.TrainTypeId)
        # self.layout.addWidget(Color("red"))
        self.refresh_button = QtWidgets.QPushButton("刷新")
        self.layout.addWidget(self.refresh_button)
        self.refresh_button.clicked.connect(self.get_data)
        self.stop = False
        tax_lib.start()
        self.refresh()

    def is_valid(self, v):
        if v["TAXTime"]["Year"] > 3000:
            return False
        if not v["TrainTypeId"].isalnum():
            return False
        return True

    def get_tax(self):
        v = tax_lib.get()
        v = json.loads(v)
        v["TrainTypeId"] = "".join(map(lambda x: chr(x), v["TrainTypeId"]))
        return v

    def get_data(self):
        v = self.get_tax()
        # while not self.is_valid(v):
        #     v = self.get_tax()
        v[
            "TAXTime"
        ] = f'{v["TAXTime"]["Year"]:04d}-{v["TAXTime"]["Month"]:02d}-{v["TAXTime"]["Day"]:02d} {v["TAXTime"]["Hour"]:02d}:{v["TAXTime"]["Minute"]:02d}:{v["TAXTime"]["Second"]:02d}'

        # print(json.dumps(v, indent=4, sort_keys=False))
        self.TaxTime.set_val(v["TAXTime"])
        self.FactRoute.set_val(v["FactRoute"])
        self.DieselSpeedAndOriginalCurrent.set_val(v["DieselSpeedAndOriginalCurrent"])
        self.Speed.set_val(v["Speed"])
        self.MaxSpeed.set_val(v["MaxSpeed"])
        self.TrainSign.set_val(v["TrainSign"])
        self.TrainState.set_val(v["TrainState"])
        self.SignNo.set_val(v["SignNo"])
        self.SignType.set_val(v["SignType"])
        self.Signpost.set_val(v["Signpost"])
        self.CarWeight.set_val(v["CarWeight"])
        self.CarLong.set_val(v["CarLong"])
        self.CarCount.set_val(v["CarCount"])
        self.TrainFlag.set_val(v["TrainFlag"])
        self.TrainNum.set_val(v["TrainNum"])
        self.SectionNo.set_val(v["SectionNo"])
        self.StationNo.set_val(v["StationNo"])
        self.DriverNo.set_val(v["DriverNo"])
        self.CopilotNo.set_val(v["CopilotNo"])
        self.EngineNo.set_val(v["EngineNo"])
        self.EngineType.set_val(v["EngineType"])
        self.DevicePlace.set_val(v["DevicePlace"])
        self.PipePressure.set_val(v["PipePressure"])
        self.TrainTypeId.set_val(v["TrainTypeId"])

    def refresh(self):
        import threading
        import time

        def inner():
            while not self.stop:
                self.get_data()
                time.sleep(1)

        threading.Thread(target=inner).start()

    def closeEvent(self, a0: QtGui.QCloseEvent) -> None:
        self.stop = True
        tax_lib.stop()


if __name__ == "__main__":
    import sys

    app = QtWidgets.QApplication(sys.argv)
    window = MainWindow()
    window.show()
    app.exec()
