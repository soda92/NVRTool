import sys

import shutil
from service import get_udisk, get_raw_conf
import os
import datetime
import time
import pdb


def get_files():
    """通过U盘下载配置, 获取要下载的文件"""

    folder = get_raw_conf("folder")

    dirnames = sorted(os.listdir(folder), reverse=False)

    channels, time_start_req, time_end_req = get_config()
    # print(channels, time_start, time_end)

    date_start = datetime.datetime(
        year=time_start_req.year, month=time_start_req.month, day=time_start_req.day
    )
    date_end = datetime.datetime(
        year=time_end_req.year, month=time_end_req.month, day=time_end_req.day
    )
    date_list = []
    if date_start > date_end:
        pass
    else:
        date_list.append(datetime.datetime.strftime(date_start, "%Y-%m-%d"))
        while date_start != date_end:
            date_start += datetime.timedelta(days=1)
            date_list.append(datetime.datetime.strftime(date_start, "%Y-%m-%d"))

    req_dirs = []
    for i in date_list:
        if i in dirnames:
            req_dirs.append(i)

    file_lists = []
    import pdb

    # pdb.set_trace()
    for dirname in req_dirs:
        files = []
        for file_name in os.listdir(os.path.join(folder, dirname)):
            if not file_name.endswith("mp4"):
                if file_name.endswith("idx"):
                    files.append(file_name)
                else:
                    continue
            list_ = file_name[:-4].split("_")
            channel_ = list_[2]
            time_ = list_[-2] + "_" + list_[-1]
            time_start_video = datetime.datetime.strptime(time_, "%Y%m%d_%H%M%S")
            time_end_video = time_start_video + datetime.timedelta(minutes=15)

            if (
                not (
                    (time_end_video <= time_start_req)
                    or (time_start_video >= time_end_req)
                )
            ) and channel_ in channels:
                files.append(file_name)
        if len(files) != 0:
            file_lists.append([dirname, files])
    return folder, file_lists


def get_config():
    """解析下载配置"""
    file = f"{get_udisk()}/license.txt"

    lines = []
    with open(file, encoding="gbk", mode="r") as f:
        lines = f.readlines()
        lines = list(filter(lambda x: "=" in x, lines))

    lines = list(map(lambda x: x.split("=")[1].split("//")[0].strip()[1:-1], lines))
    username = lines[0]
    password = lines[1]

    if username != "6A" or password != "A6":
        raise SystemError("username or password wrong")

    channels = lines[2].split(",")
    if channels == ["00"]:
        # format string: fill-align-type
        channels = [f"{str(x+1):0>2s}" for x in range(12)]

    time_start = datetime.datetime.strptime(lines[3], "%Y-%m-%d %H:%M")
    time_end = time_start + datetime.timedelta(hours=int(lines[4]))
    return channels, time_start, time_end


def get_total():
    src, arr = get_files()
    arr_all = []
    for folder, files in arr:
        for file in files:
            arr_all.append(os.path.join(src, folder, file))
    return arr_all


def copy_file_impl(callback):
    src, arr = get_files()
    _channels, config_time_start, config_time_end = get_config()
    path_name = (
        datetime.datetime.strftime(config_time_start, "%Y%m%d_%H%M")
        + "-"
        + datetime.datetime.strftime(config_time_end, "%Y%m%d_%H%M")
    )
    dest_dir = f"{get_udisk()}/LT_视频下载/{src.split('-')[-2]}/{path_name}"
    os.makedirs(dest_dir, exist_ok=True)
    arr_all = []
    for folder, files in arr:
        for file in files:
            arr_all.append(os.path.join(src, folder, file))

    # pdb.set_trace()
    for i in range(len(arr_all)):
        src = arr_all[i]
        dest = os.path.join(dest_dir, os.path.basename(arr_all[i]))
        print("src: ", src)
        print("dest: ", dest)
        shutil.copy(src, dest)

        callback(int(100 / len(arr_all) * (i + 1)))


def test(callback):
    for i in range(100):
        time.sleep(0.1)
        callback(i + 1)


from pywinauto.findwindows import find_window
import win32gui


def show_window(title):
    """将另一程序设为前台"""
    try:
        window = find_window(title_re=title)
        win32gui.ShowWindow(window, True)
        win32gui.SetForegroundWindow(window)
    except Exception as e:
        print("Error: ", e)


from PyQt5.QtCore import QPoint, QThread, pyqtSignal, Qt
from PyQt5.QtWidgets import QMainWindow, QApplication
import time
import socket
import pdb


class CopyThread(QThread):
    """复制文件线程"""

    _signal = pyqtSignal(float)

    def __init__(self):
        super(CopyThread, self).__init__()

    def callback(self, value):
        """传递进度"""
        self._signal.emit(value)

    def run(self):
        # pdb.set_trace()
        if not program_args.release:
            for i in range(100):
                time.sleep(2)
                self._signal.emit(i + 1)
        else:
            copy_file_impl(self.callback)


class ListenThread(QThread):
    """串口通讯线程, 接收隐藏显示事件"""

    _signal = pyqtSignal(str)

    def __init__(self, host, port):
        super(ListenThread, self).__init__()
        self.host = host
        self.port = port

    def run(self):
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.bind((self.host, self.port))
            s.listen(1)
            while True:
                conn, addr = s.accept()
                with conn:
                    print("Connected by", addr)
                    while True:
                        data = conn.recv(1024)
                        if not data:
                            break
                        data = data.decode()
                        # print("received: ", data)
                        data = data.split("\n")
                        if data[0].strip().startswith("GET"):
                            data = data[0].strip().split()[1][1:]
                            print("parsed: ", data)
                            self._signal.emit(data)


# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'minimized_window.ui'
#
# Created by: PyQt5 UI code generator 5.15.6
#
# WARNING: Any manual changes made to this file will be lost when pyuic5 is
# run again.  Do not edit this file unless you know what you are doing.


from PyQt5 import QtCore, QtGui, QtWidgets


class Ui_MainWindow_Minimized(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(193, 33)
        MainWindow.setStyleSheet(
            "* {\n" "background-color: black;\n" "color: white;\n" "}"
        )
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.progressBar = QtWidgets.QProgressBar(self.centralwidget)
        self.progressBar.setGeometry(QtCore.QRect(0, 0, 191, 31))
        self.progressBar.setStyleSheet(
            "QProgressBar::chunk {\n"
            "    background-color: #05B8CC;\n"
            "\n"
            "}\n"
            "\n"
            "QProgressBar{\n"
            "border: 2px solid white;\n"
            "text-align: center;\n"
            "/*border-radius: 5px;*/\n"
            'font: 14pt "Microsoft YaHei UI";\n'
            "}"
        )
        self.progressBar.setProperty("value", 24)
        self.progressBar.setObjectName("progressBar")
        self.pushButton = QtWidgets.QPushButton(self.centralwidget)
        self.pushButton.setGeometry(QtCore.QRect(10, 10, 191, 41))
        self.pushButton.setStyleSheet("background-color: rgba(0,0,0,0);")
        self.pushButton.setText("")
        self.pushButton.setObjectName("pushButton")
        MainWindow.setCentralWidget(self.centralwidget)

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "MainWindow"))


class MinimizeWindow(QMainWindow, Ui_MainWindow_Minimized):
    """最小化的下载窗口, 只显示进度条"""

    def __init__(self, *args, obj=None, **kwargs):
        super(MinimizeWindow, self).__init__(*args, **kwargs)
        self.setupUi(self)
        self.setWindowFlags(self.windowFlags() | Qt.FramelessWindowHint)
        if program_args.release:
            self.setWindowFlags(self.windowFlags() | Qt.WindowStaysOnTopHint)
        self.parent = obj

        self.move(
            QApplication.desktop().screen().rect().center()
            + QPoint(-180, 250)
            + QPoint(0, 18)
        )

        self.pushButton.clicked.connect(self.restore)

    def signal_accept(self, msg):
        """更新进度的slot"""
        self.progressBar.setValue(int(msg))
        if self.progressBar.value() == 100:
            self.parent.show()
            self.close()

    def restore(self):
        """切换到正常状态(MainWindow)"""
        self.hide()
        self.parent.show()
        show_window("太原")
        self.parent.minimized = False


# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'CopyFile.ui'
#
# Created by: PyQt5 UI code generator 5.15.6
#
# WARNING: Any manual changes made to this file will be lost when pyuic5 is
# run again.  Do not edit this file unless you know what you are doing.


from PyQt5 import QtCore, QtGui, QtWidgets


class Ui_MainWindow_Normal(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(800, 400)
        MainWindow.setStyleSheet(
            "*{\n" "background-color: black;\n" "color: white;\n" "}\n" ""
        )
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.progressBar = QtWidgets.QProgressBar(self.centralwidget)
        self.progressBar.setGeometry(QtCore.QRect(130, 210, 571, 41))
        self.progressBar.setStyleSheet(
            "QProgressBar{\n"
            "border: 2px solid white;\n"
            "/*border-radius: 5px;*/\n"
            "font-size: 20px;\n"
            'font: 24pt "Microsoft YaHei UI";\n'
            "text-align: center;\n"
            "}\n"
            "\n"
            "QProgressBar::chunk {\n"
            "    background-color: #05B8CC;\n"
            "\n"
            "    margin: 0.5px;\n"
            "}"
        )
        self.progressBar.setProperty("value", 24)
        self.progressBar.setObjectName("progressBar")
        self.label = QtWidgets.QLabel(self.centralwidget)
        self.label.setGeometry(QtCore.QRect(130, 90, 531, 51))
        self.label.setStyleSheet('font: 24pt "Microsoft YaHei UI";\n' "")
        self.label.setAlignment(QtCore.Qt.AlignCenter)
        self.label.setObjectName("label")
        self.pushButton = QtWidgets.QPushButton(self.centralwidget)
        self.pushButton.setGeometry(QtCore.QRect(300, 310, 211, 51))
        self.pushButton.setStyleSheet(
            "QPushButton{\n"
            "border: 2px solid white;\n"
            "border-radius: 10px;\n"
            'font: 15pt "Microsoft YaHei UI";\n'
            "}"
        )
        self.pushButton.setObjectName("pushButton")
        MainWindow.setCentralWidget(self.centralwidget)

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "MainWindow"))
        self.label.setText(_translate("MainWindow", "正在下载视频文件到U盘..."))
        self.pushButton.setText(_translate("MainWindow", "最小化"))


class MainWindow(QMainWindow, Ui_MainWindow_Normal):
    """正常大小的下载窗口, 显示在主程序的上方"""

    def __init__(self, *args, obj=None, **kwargs):
        super(MainWindow, self).__init__(*args, **kwargs)
        self.setupUi(self)
        if program_args.release:
            self.setWindowFlags(self.windowFlags() | Qt.FramelessWindowHint)
            self.setWindowFlags(self.windowFlags() | Qt.WindowStaysOnTopHint)
        # self.pushButton.setHidden(True)
        self.pushButton.clicked.connect(self.do_action)

        self.move(
            QApplication.desktop().screen().rect().center() - self.rect().center()
        )

        self.minimized = False
        self.close_flag = False

        self.progressBar.setProperty("value", 0)

        self.minimized_window = MinimizeWindow(obj=self)
        self.minimized_window.hide()

        if program_args.release and len(copy_file_impl.get_total()) == 0:
            self.label.setText("时间段内无文件")
            self.close_flag = True
            self.pushButton.setText("关闭")
            self.pushButton.setHidden(False)
        else:
            self.thread = CopyThread()
            self.thread._signal.connect(self.signal_accept)
            self.thread._signal.connect(self.minimized_window.signal_accept)
            self.thread.start()

            self.listen_thread = ListenThread("localhost", 55555)
            self.listen_thread._signal.connect(self.command)
            self.listen_thread.start()

    def set_path(self, path):
        self.label.setText(path)

    def signal_accept(self, msg):
        self.progressBar.setValue(int(msg))
        if self.progressBar.value() == 100:
            self.pushButton.setHidden(False)
            self.label.setText("下载完成")
            self.pushButton.setText("关闭")
            self.close_flag = True

    def do_action(self):
        """按钮动作"""
        if not self.close_flag:
            if not self.minimized:
                self.hide()
                self.minimized_window.show()
                show_window("太原")
                self.minimized = True
        else:
            QApplication.exit()

    def command(self, command):
        if command == "show":
            if self.minimized:
                self.minimized_window.show()
            else:
                self.show()
            show_window("太原")
        if command == "hide":
            self.hide()
            self.minimized_window.hide()
            show_window("太原")


if __name__ == "__main__":
    # sys.argv.extend(['--release', '1'])

    import argparse

    parser = argparse.ArgumentParser(description="Process debug options.")
    parser.add_argument("--release", type=bool, default=False, help="release mode")
    program_args = parser.parse_args()

    app = QApplication(sys.argv)
    ex = MainWindow()
    ex.show()
    show_window("太原")
    sys.exit(app.exec())
