import subprocess
import re
import shutil
import os
import glob
import win32api
import win32file


def get_version():
    line = ""
    with open("LT_WXCL/CHANGELOG.md", mode="r", encoding="gbk") as f:
        lines = f.readlines()
        lines = list(filter(lambda line: "v" in line, lines))
        line = lines[-1]
    version = re.findall(r"v[0-9\.]+[0-9]", line)[0]
    return version


def get_udisk():
    for i in win32api.GetLogicalDriveStrings().split("\x00"):
        if i == "":
            continue
        if win32file.GetDriveType(i) == win32file.DRIVE_REMOVABLE:
            return i[0] + ":"
    return None


def copy_dist_to_udisk():
    udisk = get_udisk()
    if udisk != None:
        print(f"copying to {udisk}...")
        if os.path.exists(udisk):
            for file in glob.glob(f"./nsis-build/太原*-{get_version()}.exe"):
                shutil.copy(file, udisk)
    else:
        print("cannot find udisk.")


def build():
    """build"""
    print("current dir: ", os.getcwd())
    print("copying file...")

    dirname = f"nsis-build/TaiYuan-Release-{get_version()}"
    if os.path.exists(dirname):
        shutil.rmtree(dirname)
    os.mkdir(dirname)
    for file in glob.glob("Release/*"):
        if not file.endswith("pdb"):
            if os.path.isdir(file):
                out_path = os.path.join(dirname, os.path.basename(file))
                shutil.copytree(file, out_path)
            else:
                shutil.copy(file, dirname)

    # additional files
    for file in glob.glob("ExtraFiles/*"):
        shutil.copy(file, dirname)

    print("generating script...")
    lines = []
    with open("installer.pre.nsi", mode="r", encoding="utf-8-sig") as f:
        lines = f.readlines()
    version = get_version()

    for i in range(len(lines)):
        lines[i] = lines[i].replace("{version}", version)

    with open("installer.out.nsi", mode="w", encoding="utf-8-sig") as f:
        for line in lines:
            f.write(line)

    print("building...")
    subprocess.run("makensis installer.out.nsi")
    copy_dist_to_udisk()


if __name__ == "__main__":
    build()
