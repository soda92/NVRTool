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

    print("Total: %d GiB" % (total // (2 ** 30)))
    print("Used: %d GiB" % (used // (2 ** 30)))
    print("Free: %d GiB" % (free // (2 ** 30)))


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
    free = free / 2 ** 30
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
            deleted_size > (size * (2 ** 30))
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

            if deleted_size > (size * (2 ** 30)):
                break
    return deleted_size / (2 ** 30)


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
        return tuple(map(lambda x: x / (2 ** 30), tup))

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
            data = ""

            import configparser

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


if __name__ == "__main__":
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
