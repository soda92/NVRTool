from invoke import task
import shutil
import os
import glob

@task
def build(c):
    c.run("py build-release.py")
    c.run("makensis installer.nsi")
    if os.path.exists('F:'):
        for file in glob.glob("./太原*.exe"):
            shutil.copy(file, "F:")
