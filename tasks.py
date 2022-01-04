from invoke import task
import shutil
import os
import glob
from build_release import get_version

@task
def release(c):
    dirname = f"TaiYuan-Release-{get_version()}"
    if os.path.exists(dirname):
        shutil.rmtree(dirname)
    os.mkdir(dirname)
    for file in glob.glob('Release/*'):
        if not file.endswith("pdb"):
            if os.path.isdir(file):
                out_path = os.path.join(dirname, os.path.basename(file))
                shutil.copytree(file, out_path)
            else:
                shutil.copy(file, dirname)

    c.run('py gen_installer_script.py')
    c.run("makensis installer.out.nsi")
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
