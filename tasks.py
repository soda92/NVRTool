from invoke import task
import shutil
import os
import glob
from get_version import get_version
import win32api
import win32file


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

@task
def copyconf(c):
    shutil.copy("LT_WXCLCFG.ini", "Debug")
    shutil.copy("LT_WXCLCFG.ini", "Release")

@task
def build(c):
    """build"""
    print("current dir: ", os.getcwd())
    print("copying file...")
    copyconf(c)
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
    c.run("py gen_installer_script.py")
    print("building...")
    c.run("makensis installer.out.nsi")
    copy_dist_to_udisk()


# copy built file
@task
def copy(c):
    copy_dist_to_udisk()
