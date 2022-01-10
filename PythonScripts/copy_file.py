import sys

sys.path.append("packages")
sys.path.append(".")
sys.argv.extend(['--release', '1'])

import argparse

parser = argparse.ArgumentParser(description="Process debug options.")
parser.add_argument("--release", type=bool, default=False, help="release mode")
program_args = parser.parse_args()


from pywinauto.findwindows import find_window
import win32gui


def show_window(title):
    try:
        window = find_window(title_re=title)
        win32gui.ShowWindow(window, True)
        win32gui.SetForegroundWindow(window)
    except Exception as e:
        print("Error: ", e)


import copy_file_impl
from PyQt5.QtCore import QPoint, QThread, pyqtSignal, Qt
from PyQt5.QtWidgets import QMainWindow, QApplication
from copy_file_window import Ui_MainWindow
import minimized_window
import time
import socket
import pdb


class CopyThread(QThread):
    _signal = pyqtSignal(float)

    def __init__(self):
        super(CopyThread, self).__init__()

    def callback(self, value):
        self._signal.emit(value)

    def run(self):
        # pdb.set_trace()
        if not program_args.release:
            for i in range(100):
                time.sleep(2)
                self._signal.emit(i + 1)
        else:
            copy_file_impl.copy_file_impl(self.callback)


class ListenThread(QThread):
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
                        print("received: ", data)
                        self._signal.emit(data)


class MinimizeWindow(QMainWindow, minimized_window.Ui_MainWindow):
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
        self.progressBar.setValue(int(msg))
        if self.progressBar.value() == 100:
            self.parent.show()
            self.close()

    def restore(self):
        self.hide()
        self.parent.show()
        show_window('太原')
        self.parent.minimized = False


class MainWindow(QMainWindow, Ui_MainWindow):
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
            self.pushButton.setText('关闭')
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
        if not self.close_flag:
            if not self.minimized:
                self.hide()
                self.minimized_window.show()
                show_window('太原')
                self.minimized = True
        else:
            QApplication.exit()

    def command(self, command):
        if command == "show":
            if self.minimized:
                self.minimized_window.show()
            else:
                self.show()
            show_window('太原')
        if command == "hide":
            self.hide()
            self.minimized_window.hide()
            show_window('太原')


if __name__ == "__main__":
    app = QApplication(sys.argv)
    ex = MainWindow()
    ex.show()
    show_window('太原')
    sys.exit(app.exec())
