import shutil
from check_disk import get_udisk
from get_files import get_files
import os
import datetime
import time
import pdb

from parse_config import get_config


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
    path_name = datetime.datetime.strftime(config_time_start, "%Y%m%d_%H%M")+'-' +\
        datetime.datetime.strftime(config_time_end, "%Y%m%d_%H%M")
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

        callback(int(100/len(arr_all)*(i+1)))


def test(callback):
    for i in range(100):
        time.sleep(0.1)
        callback(i+1)


if __name__ == '__main__':
    copy_file_impl(print)
