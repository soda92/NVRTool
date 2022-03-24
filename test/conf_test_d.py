import subprocess
import pytest
import requests
import pathlib

project_dir = pathlib.Path(__file__).resolve().parent.parent
bin_dir = project_dir.joinpath("bin")


def get(key: str) -> str:
    r = requests.get(f"http://127.0.0.1:5000/{key}")
    if r.status_code == 200:
        return r.text
    else:
        raise SystemError()


import os

import time


@pytest.fixture(autouse=True, scope="session")
def server():
    os.chdir(str(bin_dir))
    subprocess.Popen("py ../service.py".split())
    time.sleep(7)
    yield
    get("shutdown")
    time.sleep(7)


import configparser


def set_conf(key: str, value: str) -> None:
    config = configparser.ConfigParser()
    file = "程序配置.ini"
    config.read(file)
    config["Core"][key] = value
    with open(file, "w") as configfile:
        config.write(configfile)
    get("reload")


def test_conf():
    set_conf("TrainNum", "TS123456")
    assert get("conf/TrainNum") == "TS123456"
    set_conf("TrainNum", "STUKC")
    assert get("conf/TrainNum") == "STUKC"
    set_conf("Local", "A")
    assert get("local") == "A"
    set_conf("Local", "B")
    assert get("local") == "B"
