from invoke import task
import shutil

# Python program to find SHA256 hash string of a file
import hashlib

# ref:
# https://www.quickprogrammingtips.com/python/how-to-calculate-sha256-hash-of-a-file-in-python.html
def get_hash(filename):
    sha256_hash = hashlib.sha256()
    with open(filename, "rb") as f:
        # Read and update hash string value in blocks of 4K
        for byte_block in iter(lambda: f.read(4096), b""):
            sha256_hash.update(byte_block)
        return sha256_hash.hexdigest()


def gen_hash():
    with open("config-hash.txt", mode="w") as f:
        hash = get_hash("../TaiYuan/Release/LT_WXCLCFG.ini")
        f.write(hash)
        f.write("\n")


def copy_hash():
    shutil.copy("config-hash.txt", "../TaiYuan/Release/")


@task
def build(c):
    c.run("pyuic5 minimized_window.ui -o minimized_window.py")
    c.run("pyuic5 CopyFile.ui -o copy_file_window.py")


@task
def debug(c, wclient=False, wgui=False):
    if not wclient:
        c.run("pyinstaller client.py --onefile")
    else:
        c.run("pyinstaller client.py --onefile --windowed")
    if not wgui:
        c.run("pyinstaller copy_file.py --onefile --hidden-import=PyQt5.sip")
    else:
        c.run("pyinstaller copy_file.py --onefile --hidden-import=PyQt5.sip --windowed")

    c.run("pyinstaller update-profile.py --onefile")
    c.run("pyinstaller launcher.py --onefile")

    shutil.copy("dist/update-profile.exe", "../TaiYuan/Debug")
    shutil.copy("dist/launcher.exe", "../TaiYuan/Debug")

    shutil.copy("dist/client.exe", "../TaiYuan/Debug")
    shutil.copy("dist/copy_file.exe", "../TaiYuan/Debug")

    gen_hash()
    copy_hash()


@task
def release(c):
    c.run("pyinstaller client.py --onefile --windowed")
    c.run("pyinstaller copy_file.py --onefile --hidden-import=PyQt5.sip --windowed")

    c.run("pyinstaller update-profile.py --onefile --windowed")
    c.run("pyinstaller launcher.py --onefile")

    shutil.copy("dist/update-profile.exe", "../TaiYuan/Release")
    shutil.copy("dist/launcher.exe", "../TaiYuan/Release")

    shutil.copy("dist/client.exe", "../TaiYuan/Release")
    shutil.copy("dist/copy_file.exe", "../TaiYuan/Release")

    gen_hash()
    copy_hash()
