import subprocess
import os
import logging
import sys
import os

import socket

hostname = socket.gethostname()
is_test_env = False
if hostname != "user-PC":
    is_test_env = True

from typing import Tuple

"""
自动发现旧的配置文件 (仅读取车号和AB节)

旧配置文件名称: LT_WXCLCFG.ini
新配置文件名称: 程序配置.ini
    覆盖默认配置文件: Default.ini

"""


def read_ini(file, encoding="gbk"):
    """读取ini，形成kv列表"""
    data = []
    with open(file, encoding=encoding) as f:
        lines = f.readlines()
        lines = [line.strip() for line in lines]
        data = lines

    for i in range(len(data)):
        if "=" in data[i]:
            data[i] = data[i].split("=")
    return data


def get_value(data, key):
    """获取某一key对应的值"""
    for line in data:
        if type(line) == str:
            continue
        elif type(line) == list and len(line) == 2:
            if line[0] == key:
                return line[1]
    raise ValueError(f"{key} not found")


def key_exists(data, key):
    """是否有该key"""
    exist = False
    for line in data:
        if type(line) == str:
            continue
        elif type(line) == list and len(line) == 2:
            if line[0] == key:
                exist = True
                break
    return exist


def find_old_config():
    """获取旧配置"""
    oldrel_path = "D:/TaiYuan-Release-v1.2.10/LT_WXCLCFG.ini"
    if os.path.exists(oldrel_path):
        return oldrel_path
    return None


def get_old_val() -> Tuple[str, str]:
    oldconf = find_old_config()
    if oldconf is not None:
        train_number = ""
        local_a_or_b = ""
        if os.path.basename(oldconf).endswith("ini"):
            data = read_ini(oldconf)
            if key_exists(data, "TrainNum"):
                train_number = get_value(data, "TrainNum")
            if key_exists(data, "Local"):
                local_a_or_b = get_value(data, "Local")[1]
        return train_number, local_a_or_b
    else:
        return None, None


def update_profile():
    """更新配置"""
    # https://docs.python.org/3/library/configparser.html
    import configparser

    config = configparser.ConfigParser()

    train_number, local_a_or_b = get_old_val()
    if train_number is not None and local_a_or_b is not None:
        config["Core"] = {"TrainNum": train_number, "Local": local_a_or_b}
        with open("程序配置.ini", "w") as configfile:
            config.write(configfile)


import shutil
import win32api
import win32file


def print_disk(path):
    total, used, free = shutil.disk_usage(path)

    print("Total: %d GiB" % (total // (2**30)))
    print("Used: %d GiB" % (used // (2**30)))
    print("Free: %d GiB" % (free // (2**30)))


def get_udisk():
    """判断是否为U盘"""
    for i in win32api.GetLogicalDriveStrings().split("\x00"):
        if i == "":
            continue
        if win32file.GetDriveType(i) == win32file.DRIVE_REMOVABLE:
            return i[0] + ":"
    raise SystemError("Cannot find udisk")


def get_hdd():
    """判断是否为硬盘"""
    for i in win32api.GetLogicalDriveStrings().split("\x00"):
        if i == "" or i[0] == "C" or i[0] == "D":
            continue
        if win32file.GetDriveType(i) == win32file.DRIVE_FIXED:
            return i[0] + ":"
    raise SystemError("Cannot find HDD")


import shutil
import os
import time

import logging
import requests

logging.basicConfig(
    filename="D:/程序信息记录.txt",
    level=logging.INFO,
    format="%(asctime)s: %(levelname)7s: [%(name)s]: %(message)s",
    datefmt="%Y-%m-%d %H:%M:%S",
)


def read_ini(file, encoding="gbk"):
    """读取配置文件，返回data"""
    data = []
    with open(file, encoding=encoding) as f:
        lines = f.readlines()
        lines = [line.strip() for line in lines]
        data = lines

    for i in range(len(data)):
        if "=" in data[i]:
            data[i] = data[i].split("=")
    return data


def all_keys(data):
    """返回data中的所有key"""
    keys = []
    for line in data:
        if type(line) == str:
            continue
        elif type(line) == list and len(line) == 2:
            keys.append(line[0])
    return keys


def get_value(data, key):
    """获取指定Key的值"""
    for line in data:
        if type(line) == str:
            continue
        elif type(line) == list and len(line) == 2:
            if line[0] == key:
                return line[1]
    raise ValueError(f"{key} not found")


def get_conf(key):
    """获取配置"""
    return requests.get(f"http://localhost:5000/conf/{key}").text


def get_raw_conf(path):
    """获取路径的值"""
    return requests.get(f"http://localhost:5000/{path}").text


from pathlib import Path


def get_size(folder: str) -> int:
    """计算文件夹大小
    来自: https://stackoverflow.com/a/57134202/12291425"""
    return sum(p.stat().st_size for p in Path(folder).rglob("*"))


def filesize(size: int) -> str:
    """计算附带单位的大小"""
    for unit in ("B", "K", "M", "G", "T"):
        if size < 1024:
            break
        size /= 1024
    return f"{size:.1f}{unit}"


def get_disk():
    """读取配置文件的硬盘"""
    import requests

    r = requests.get("http://localhost:5000/conf/Disk")
    if r.status_code == 200:
        return r.text
    else:
        raise SystemError("cannot read network conf")


def get_folder():
    """读取程序路径"""
    disk = requests.get("http://localhost:5000/conf/Disk").text
    folder_list = []
    for item in os.listdir(f"{disk}/"):
        if item == "$RECYCLE.BIN" or item == "System Volume Information":
            continue
        if os.path.isdir(item):
            logging.info(f"{disk}/{item}: ", filesize(get_size(f"{disk}/{item}")))
            folder_list.append(item)

    train_num = requests.get("http://localhost:5000/conf/TrainNum").text
    folder = f"{disk}:/LT-VIDEO-{train_num}-北京蓝天多维"
    if not os.path.exists(folder):
        raise SystemError("No Folder")
    return folder


def get_unused_folder():
    """计算要删除的文件夹
    当硬盘里有其他车的视频文件时，优先删除其他车的视频，最后删除本车的
    """
    disk = get_conf("Disk")

    folder_list = []
    basedir = f"{disk}:/"
    for item in os.listdir(basedir):
        if item == "$RECYCLE.BIN" or item == "System Volume Information":
            continue
        item_path = os.path.join(basedir, item)
        if os.path.isdir(item_path):
            # 忽略非视频文件夹
            if not "LT" in item_path:
                continue
            folder_list.append(item_path)
            item_size = filesize(get_size(item_path))
            logging.info(f"{item_path}: {item_size}")

    train_num = get_conf("TrainNum")
    folder = f"{disk}:/LT-VIDEO-{train_num}-北京蓝天多维"
    if len(folder_list) == 1 and folder_list[0] == folder:
        return folder
    for i in folder_list:
        if i != folder:
            return i
    raise SystemError("No Folder")


def get_disk_free(disk):
    """获取磁盘剩余空间"""
    free = shutil.disk_usage(f"{disk}:/").free
    free = free / 2**30
    return free


def get_old_files(folder, num):
    """最旧的num个文件"""
    files = []
    for d in sorted(os.listdir(folder)):
        if len(files) >= num:
            break
        if not os.path.isdir(os.path.join(folder, d)):
            continue
        for entry in sorted(os.listdir(os.path.join(folder, d))):
            if not os.path.isfile(os.path.join(folder, d, entry)):
                continue
            files.append(os.path.join(folder, d, entry))
            if len(files) >= num:
                break
    return files


def remove_empty_dirs(folder):
    """删除空目录"""
    for fold in os.listdir(folder):
        file_list = os.listdir(os.path.join(folder, fold))
        if len(file_list) == 0 or (len(file_list) == 1 and file_list[0][-3:] == "idx"):
            logging.info(f"deleting folder: {os.path.join(folder, fold)}")
            shutil.rmtree(os.path.join(folder, fold))

    train_num = get_conf("TrainNum")
    disk = get_conf("Disk")
    s = f"{disk}:/LT-VIDEO-{train_num}-北京蓝天多维"
    if len(os.listdir(folder)) == 0:
        if os.path.basename(s) != os.path.basename(folder):
            os.rmdir(folder)


def delete_some(folder, size):
    """删除指定大小的文件"""
    deleted_size = 0
    cannot_delete_files = 0
    while True:
        if (
            deleted_size > (size * (2**30))
            or len(get_old_files(folder, 10)) == cannot_delete_files
        ):
            break
        for file in get_old_files(folder, 10):
            try:
                os.remove(file)
                deleted_size += os.path.getsize(file)
                logging.info(f"deleting file: {file}")
            except Exception as e:
                cannot_delete_files += 1
                continue

            if deleted_size > (size * (2**30)):
                break
    return deleted_size / (2**30)


global_exit = False


def delete_file():
    size = 5
    import sys

    if len(sys.argv) == 2:
        size = int(sys.argv[1])

    while True:
        if global_exit:
            break
        try:
            disk = get_conf("Disk")
            folder = get_unused_folder()
            if get_disk_free(disk) < size:
                delete_some(folder, 1)
            remove_empty_dirs(folder)
            time.sleep(10)
        except:
            time.sleep(1)
            continue


import multiprocessing


def serve(q: multiprocessing.Queue) -> None:
    """
    相关服务

    提供：

    配置服务
    日志服务（未使用）
    磁盘信息服务

    """

    # from: https://stackoverflow.com/questions/49586657/how-to-return-json-from-sqlite-in-flask-builtins-typeerror
    import datetime
    from flask import (
        Flask,
        g,
        jsonify,
    )
    import sqlite3

    app = Flask(__name__)

    DATABASE = "log.sqlite3"
    _conf = None
    _addrs = None

    def get_db():
        """打开数据库"""
        db = getattr(g, "_database", None)
        if db is None:
            db = g._database = sqlite3.connect(DATABASE)
            db.row_factory = sqlite3.Row
        return db

    @app.teardown_appcontext
    def close_connection(exception):
        db = getattr(g, "_database", None)
        if db is not None:
            db.close()

    @app.route("/add/<message>/")
    def add_message(message):
        """添加日志"""
        db = get_db()
        c = db.cursor()
        c.execute(
            "INSERT INTO log (level, message, time) values (?, ?, ?);",
            (1, message, datetime.datetime.now()),
        )
        db.commit()
        return jsonify({"result": "success"})

    @app.route("/pages")
    def get_pages():
        """页面数量"""
        db = get_db()
        c = db.cursor()
        r = c.execute("SELECT COUNT(*) from log;").fetchall()
        result = r[0][0]
        result = result // 10 + 1
        return jsonify({"result": result})

    @app.route("/page/<pagenum>")
    def get_page(pagenum):
        """某一页"""
        offset = (int(pagenum) - 1) * 10
        db = get_db()
        c = db.cursor()
        r = c.execute(
            "SELECT id, level, message, time from log limit 10 offset ?;", [offset]
        )
        data = r.fetchall()
        data_array = []
        for row in data:
            obj = dict()
            obj["id"] = row[0]
            obj["level"] = row[1]
            obj["message"] = row[2]
            obj["time"] = row[3]
            data_array.append(obj)
        return jsonify(data_array)

    import os

    @app.route("/is_record/<disk>")
    def parse_config(disk):
        """判断U盘中的配置是否为录像配置"""
        data = dict()
        data["record"] = False
        p = os.path.join(f"{disk}:/", "license.txt")
        if not os.path.exists(p):
            return jsonify(data)

        content = ""
        with open(p, encoding="gbk") as f:
            content = f.read()

        if "TimeFrom" in content:
            data["record"] = False
        else:
            data["record"] = True
        return jsonify(data)

    @app.route("/parse_channels/<disk>")
    def parse_channels(disk):
        """解析U盘录像通道, 支持6个, 00代表0-6"""
        data = dict()
        channels = [False for i in range(6)]
        p = os.path.join(f"{disk}:/", "license.txt")
        if not os.path.exists(p):
            return jsonify(data)

        content = ""
        with open(p, encoding="gbk") as f:
            content = f.read()

        line = ""
        for i in content.split("\n"):
            if "Channel" in i:
                line = i
        channel_str = line.split("=")[1].strip().split()[0][1:-1]
        if channel_str == "00":
            channels = [True for i in range(6)]
        else:
            for i in channel_str.split(","):
                channels[int(i) - 1] = True

        data["channels"] = channels
        return jsonify(data)

    def get_size_info():
        """获取磁盘详细信息"""
        import shutil

        disk = get_conf("Disk")

        tup = (total, used, free) = shutil.disk_usage(disk + ":")
        return tuple(map(lambda x: x / (2**30), tup))

    @app.route("/size")
    def disk_size():
        """可用空间"""
        total, used, free = get_size_info()
        data = {"total": total, "used": used, "free": free}
        return jsonify(data)

    @app.route("/conf/<key>")
    def get_conf(key):
        nonlocal _conf
        return str(_conf[key])

    import netifaces

    # TODO 通过配置修改IP
    @app.route("/local")
    def get_local():
        nonlocal _conf, _addrs
        # for i in _addrs:
        #     if i == "192.168.104.200":
        #         return "A"
        #     if i == "192.168.104.201":
        #         return "B"
        return _conf["Local"]

    @app.route("/folder")
    def get_folder():
        """程序录像路径"""
        global _conf
        disk = _conf["Disk"]
        train_num = _conf["TrainNum"]
        return f"{disk}:/LT-VIDEO-{train_num}-北京蓝天多维"

    @app.route("/cam/<ipcnum>")
    def get_addr(ipcnum):
        ipcnum = int(ipcnum)
        env_addr = os.environ.get(f"ipc{ipcnum}")
        if env_addr is not None and env_addr != "":
            return env_addr
        if 1 <= ipcnum <= 6:
            return f"rtsp://admin:hk123456@192.168.104.7{ipcnum-1}:554/Streaming/Channels/101"
        if 7 <= ipcnum <= 12:
            return f"rtsp://admin:hk123456@192.168.104.8{ipcnum-7}:554/Streaming/Channels/101"
        return ""

    @app.route("/test")
    def result_test():
        return "test"

    @app.route("/test/zh_hans")
    def test_gbk():
        return "中文"

    def shutdown_server():
        q.put("yes")

    @app.get("/shutdown")
    def shutdown():
        shutdown_server()
        return "Server shutting down..."

    import configparser

    @app.get("/reload")
    def reload_conf():
        nonlocal _conf
        config = configparser.ConfigParser()
        config.read("Default.ini")
        config.read("程序配置.ini")
        _conf = config["Core"]
        return "success"

    def server():
        nonlocal app, _conf, _addrs
        with app.app_context():
            db = get_db()
            c = db.cursor()
            c.execute(
                "SELECT name FROM sqlite_master WHERE type='table' AND name='log';"
            )
            result = c.fetchall()
            if len(result) != 0:
                # print('Table exists.')
                pass
            else:
                c.execute(
                    "CREATE TABLE log(id INTEGER PRIMARY KEY AUTOINCREMENT, level int, message varchar(100), time timestamp);"
                )

            config = configparser.ConfigParser()

            config.read("Default.ini")
            config.read("程序配置.ini")
            _conf = config["Core"]
            addrs = []
            for i in netifaces.interfaces():
                try:
                    addrs.append(netifaces.ifaddresses(i)[netifaces.AF_INET][0]["addr"])
                except KeyError:
                    pass
            _addrs = addrs
        app.run()

    server()


import subprocess
import psutil
import logging
import time
import os


class RecordService:
    def __init__(self, ip) -> None:
        self.ip = ip
        directory = ""
        train_num = ""
        self.name = "本机"
        args = f"ffmpeg -i rtsp://admin:hk123456@{self.ip}:554/Streaming/Channels/101 -c copy -map 0:v:0 -map 0:a:0 -segment_time 00:15:00 -f segment -strftime 1 {directory}%Y%m%d_%H%M%S_{train_num}_{self.name}.mp4"
        self.process = subprocess.Popen(
            args.split(),
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
        )
        self.pid = self.process.pid
        self.process2 = psutil.Process(self.pid)

    def is_alive(self):
        return self.process2.is_running()

    def terminate(self):
        self.process2.terminate()

    def get_output(self):
        output, error = self.process.communicate()
        return output, error

    def end(self):
        self.process.stdin.write(b"q\n")
        output, error = self.process.communicate()
        return output, error


def test_record():
    r = RecordService("192.168.104.70")
    logging.info(r.is_alive())
    time.sleep(30)
    output, err = r.end()

    time.sleep(1)
    logging.info(r.is_alive())

    print(err.decode())
    if r.is_alive():
        r.terminate()
    time.sleep(1)
    logging.info(r.is_alive())


import sys
import shutil
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

        if program_args.release and len(get_total()) == 0:
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


def copy_file():
    # sys.argv.extend(['--release', '1'])

    import argparse

    parser = argparse.ArgumentParser(description="Process debug options.")
    parser.add_argument("--release", type=bool, default=False, help="release mode")
    global program_args
    program_args = parser.parse_args()

    app = QApplication(sys.argv)
    ex = MainWindow()
    ex.show()
    show_window("太原")
    sys.exit(app.exec())


# http://timgolden.me.uk/python/win32_how_do_i/detect-device-insertion.html
import multiprocessing
import win32api, win32con, win32gui

#
# Device change events (WM_DEVICECHANGE wParam)
#
DBT_DEVICEARRIVAL = 0x8000
DBT_DEVICEQUERYREMOVE = 0x8001
DBT_DEVICEQUERYREMOVEFAILED = 0x8002
DBT_DEVICEMOVEPENDING = 0x8003
DBT_DEVICEREMOVECOMPLETE = 0x8004
DBT_DEVICETYPESSPECIFIC = 0x8005
DBT_CONFIGCHANGED = 0x0018

import os


def get_disks():
    disks = set()
    for c in list("ABCDEFGHIJKLMN"):
        if os.path.exists(f"{c}:/"):
            disks.add(c)
    return disks


class Notification:
    def __init__(self):
        self.disks = get_disks()
        message_map = {win32con.WM_DEVICECHANGE: self.onDeviceChange}

        wc = win32gui.WNDCLASS()
        hinst = wc.hInstance = win32api.GetModuleHandle(None)
        wc.lpszClassName = "DeviceChangeDemo"
        wc.style = win32con.CS_VREDRAW | win32con.CS_HREDRAW
        wc.hCursor = win32gui.LoadCursor(0, win32con.IDC_ARROW)
        wc.hbrBackground = win32con.COLOR_WINDOW
        wc.lpfnWndProc = message_map
        classAtom = win32gui.RegisterClass(wc)
        style = win32con.WS_OVERLAPPED | win32con.WS_SYSMENU
        self.hwnd = win32gui.CreateWindow(
            classAtom,
            "Device Change Demo",
            style,
            0,
            0,
            win32con.CW_USEDEFAULT,
            win32con.CW_USEDEFAULT,
            0,
            0,
            hinst,
            None,
        )

    def on_in(self, func):
        self.in_func = func

    def on_out(self, func):
        self.out_func = func

    def onDeviceChange(self, hwnd, msg, wparam, lparam):
        #
        # WM_DEVICECHANGE:
        #  wParam - type of change: arrival, removal etc.
        #  lParam - what's changed?
        #    if it's a volume then...
        #  lParam - what's changed more exactly
        #
        if wparam == DBT_DEVICEARRIVAL:
            # print("Something's arrived")
            import time

            time.sleep(0.3)
            n_disks = get_disks()

            dev_in = n_disks - self.disks
            if len(dev_in) != 0:
                self.in_func(dev_in)
            self.disks = n_disks

        if wparam == DBT_DEVICEREMOVECOMPLETE:
            # print("Something's leaving")
            n_disks = get_disks()
            dev_out = self.disks - n_disks
            if len(dev_out) != 0:
                self.out_func(dev_out)
            self.disks = n_disks

        return 1


class device_monitor:
    def __init__(self, q: multiprocessing.Queue) -> None:
        self.w = Notification()
        self.q = q

    def on_in(self, func):
        self.w.on_in(func)

    def on_out(self, func):
        self.w.on_out(func)

    def run_loop(self):
        main_thread_id = win32api.GetCurrentThreadId()
        self.q.put(main_thread_id)
        win32gui.PumpMessages()


def dev(q: multiprocessing.Queue) -> None:
    d = device_monitor(q)
    d.on_in(print)
    d.on_out(print)
    d.run_loop()


def thread_quit(thread_id: int):
    win32api.PostThreadMessage(thread_id, win32con.WM_QUIT, 0, 0)


def test_device_detect():
    q = multiprocessing.Queue()
    process = multiprocessing.Process(target=dev, args=(q,))
    process.start()
    thread_id = q.get(block=True)
    import time

    time.sleep(30)
    thread_quit(thread_id)


def all_launcher():
    logging.basicConfig(
        level=logging.DEBUG,
        format="%(asctime)s: %(levelname)7s: [%(name)s]: %(message)s",
        datefmt="%Y-%m-%d %H:%M:%S",
    )
    logging.info(f"Current directory: {os.getcwd()}")
    logging.info(f"Current file: {__file__}")

    if not is_test_env:
        if not os.path.exists("程序配置.ini"):
            update_profile()

    import threading
    import multiprocessing

    q = multiprocessing.Queue()
    server_process = multiprocessing.Process(target=serve, args=(q,))
    server_process.start()

    t2 = threading.Thread(target=delete_file)
    t2.start()

    def wait_exit():
        print("waiting")
        q.get(block=True)
        server_process.terminate()
        global global_exit
        global_exit = True

    if is_test_env:
        wait_exit()
    else:
        import ctypes, sys

        def is_admin():
            try:
                return ctypes.windll.shell32.IsUserAnAdmin()
            except:
                return False

        if not is_admin():
            # Re-run the program with admin rights
            ctypes.windll.shell32.ShellExecuteW(
                None, "runas", sys.executable, " ".join(sys.argv), None, 1
            )
        else:
            subprocess.run("FireVideo.exe")
            wait_exit()


if __name__ == "__main__":
    all_launcher()
