from invoke import task
import shutil
import os
import glob
from build_release import get_version

@task
def release(c):
    c.run("py build_release.py")
    c.run("makensis installer.nsi")
    if os.path.exists('F:'):
        for file in glob.glob(f"./太原*-{get_version()}.exe"):
            shutil.copy(file, "F:")

@task
def debug(c):
    c.run("py build_debug.py")
    c.run("makensis installer-debug.nsi")
    if os.path.exists('F:'):
        for file in glob.glob(f"./太原*-{get_version()}-debug.exe"):
            shutil.copy(file, "F:")
