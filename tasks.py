from invoke import task
import shutil
import os
import glob
from get_version import get_version

@task
def build(c):
    print('current dir: ', os.getcwd())
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
    breakpoint()
    # add python
    shutil.copy('python-3.8.10.exe', dirname)

    c.run('py gen_installer_script.py')
    c.run("makensis installer.out.nsi")
    if os.path.exists('F:'):
        for file in glob.glob(f"./太原*-{get_version()}.exe"):
            shutil.copy(file, "F:")
