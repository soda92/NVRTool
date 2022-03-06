import os
import subprocess
import pathlib

current_dir = pathlib.Path(__file__).resolve().parent
os.chdir(current_dir)


def create_disk():
    """创建空磁盘"""
    vdisk_file = "D:\\vdisk.vhd"
    if os.path.exists(vdisk_file):
        os.remove(vdisk_file)
    subprocess.run("diskpart /s create_vdisk.txt".split())


def remove_disk():
    subprocess.run("diskpart /s detach_vdisk.txt".split())
    vdisk_file = "D:\\vdisk.vhd"
    if os.path.exists(vdisk_file):
        os.remove(vdisk_file)


if __name__ == "__main__":
    create_disk()
    import time

    time.sleep(10)
    remove_disk()
