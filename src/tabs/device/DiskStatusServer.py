import logging
import socket
from flask import Flask, jsonify
import shutil
import os
import requests
from PyQt6 import QtCore
from typing import List
from dataclasses import dataclass
import json

logger = logging.getLogger(__name__)


def ping_server(server: str, port: int, timeout=3):
    """ping server"""
    try:
        socket.setdefaulttimeout(timeout)
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((server, port))
    except OSError:
        return False
    else:
        s.close()
        return True


app = Flask(__name__)

config_ = None
path_ = None
location_ = None


@app.route("/size")
def disk_size():
    """可用空间"""
    global config_, path_, location_
    if path_ == None:
        if config_ == None:
            logging.error("config not initialized")
            return
        else:
            path_ = config_["record_path_root"]
            for group in config_["groups"]:
                if group["current"]:
                    location_ = group["name"]
    data = DiskStatus.get(path_, location_)
    return jsonify(data)


# https://realpython.com/python-pyqt-qthread/#using-qthread-vs-pythons-threading
class Worker(QtCore.QObject):
    finished = QtCore.pyqtSignal()

    def run(self):
        app.run(host="0.0.0.0", port=5000)
        self.finished.emit()


@dataclass
class DiskStatus:
    location: str = ""
    total: str = ""
    used: str = ""
    free: str = ""
    error: bool = True

    @classmethod
    def get(cls, path: str, location: str):
        total, used, free = 0, 0, 0
        error = True
        if os.path.exists(path):
            total, used, free = shutil.disk_usage(path=path)
            error = False
        total, used, free = list(map(lambda x: f"{x/2**30:.2f}G", (total, used, free)))
        instance = cls(
            total=total, used=used, free=free, error=error, location=location
        )
        return instance


import pydevd


class StatusReporterWorker(QtCore.QObject):
    result_signal = QtCore.pyqtSignal(list)

    def __init__(self, config) -> None:
        super(StatusReporterWorker, self).__init__()
        pydevd.settrace(suspend=False)
        self.config = config

    def get(self):
        pydevd.settrace(suspend=False)
        status_all = []
        proxies = {
            "http": "",
            "https": "",
        }
        for group in self.config["groups"]:
            address = group["address"]
            if group["current"]:
                address = "127.0.0.1"
            try:

                r = requests.get(
                    f"http://{address}:5000/size", timeout=3, proxies=proxies
                )
            except:
                logger.info(f"{group['name']}通讯失败")
                status_all.append(DiskStatus())
            else:
                try:
                    data = json.loads(r.text)
                except:
                    logger.exception("json parse error")
                    status_all.append(DiskStatus())
                else:
                    status_all.append(DiskStatus(**data))
        return status_all

    def refresh(self):
        data = self.get()
        self.result_signal.emit(data)

    def start_routine(self):
        self.timer = QtCore.QTimer()
        self.timer.timeout.connect(self.refresh)
        self.timer.start(3000)


class StatusReporter(QtCore.QObject):
    result_signal = QtCore.pyqtSignal(list)

    def __init__(self, config) -> None:
        super(StatusReporter, self).__init__()
        self.config = config
        self.thread = QtCore.QThread()
        self.worker = StatusReporterWorker(config)
        self.worker.moveToThread(self.thread)
        self.thread.started.connect(self.worker.start_routine)
        self.worker.result_signal.connect(self.result_signal)

    def start(self):
        self.thread.start()


class DiskStatusServer(QtCore.QObject):
    thread_completed_signal = QtCore.pyqtSignal()

    def __init__(self, config):
        super(DiskStatusServer, self).__init__()
        global config_
        config_ = config

        for group in config["groups"]:
            if group["current"]:
                self.current_group = group["name"]

        self.thread = QtCore.QThread()
        self.worker = Worker()
        self.worker.moveToThread(self.thread)
        self.thread.started.connect(self.worker.run)
        self.worker.finished.connect(self.log)
        self.worker.finished.connect(self.thread.quit)
        self.worker.finished.connect(self.worker.deleteLater)
        self.thread.finished.connect(self.thread.deleteLater)

    def start(self):
        self.thread.start()

    def log(self):
        logger.warning("status server exited.")
