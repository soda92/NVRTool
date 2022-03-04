import os
import datetime
import shutil

from delete_file import delete_some

"""删除文件测试"""

conf = "D:/"
disk = "E:/"
dst_folder = f"{disk}LT-VIDEO-SS91456-北京蓝天多维"


def get_free():
    """剩余空间"""
    free = shutil.disk_usage(disk).free
    return free / (2 ** 30)


def setup():
    """创建测试文件"""
    if not os.path.exists(disk):
        raise SystemError("no disk")
    date = datetime.datetime.now()
    date_str = datetime.datetime.strftime(date, "%Y-%m-%d")
    while get_free() > 6:
        folder = os.path.join(dst_folder, date_str)
        os.makedirs(folder, exist_ok=True)
        files = [
            os.path.join(folder, filename)
            for filename in ["{}.mp4".format(p) for p in range(1, 7)]
        ]
        for file in files:
            with open(file, "wb") as out:
                out.truncate(500 * 1024 * 1024)
        date = date - datetime.timedelta(days=1)
        date_str = datetime.datetime.strftime(date, "%Y-%m-%d")


from delete_file import delete_some, remove_empty_dirs


def test_delete_some():
    """测试删除文件"""
    setup()
    free1 = shutil.disk_usage(disk).free
    delete_some(dst_folder, 4)
    free2 = shutil.disk_usage(disk).free
    size_deleted = (free2 - free1) / (2 ** 30)
    assert 5 > size_deleted > 4


if __name__ == "__main__":
    setup()
    # delete_some(dst_folder, 4)
    # remove_empty_dirs(dst_folder)
