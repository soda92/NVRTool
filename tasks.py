from invoke import task
import shutil
import os
import glob
from get_version import get_version

'''build'''
@task
def build(c):
    print("current dir: ", os.getcwd())
    print("copying file...")
    dirname = f"TaiYuan-Release-{get_version()}"
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

    # add python
    shutil.copy("python-3.8.10-amd64.exe", dirname)
    # platform info
    shutil.copy("platform_info.py", dirname)

    print("generating script...")
    c.run("py gen_installer_script.py")
    print("building...")
    c.run("makensis installer.out.nsi")
    if os.path.exists("F:"):
        for file in glob.glob(f"./太原*-{get_version()}.exe"):
            shutil.copy(file, "F:")


# copy built file
@task
def copy(c):
    if os.path.exists("F:"):
        for file in glob.glob("./太原*.exe"):
            print("copying ", file)
            shutil.copy(file, "F:")
