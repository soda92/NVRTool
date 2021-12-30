import glob
import shutil
import os
import zipfile
import re


def zipdir(path, ziph):
    # ziph is zipfile handle
    for root, dirs, files in os.walk(path):
        for file in files:
            ziph.write(os.path.join(root, file),
                       os.path.relpath(os.path.join(root, file),
                                       os.path.join(path, '..')))


def get_version():
    line = ''
    with open("LT_WXCL/CHANGELOG.md", mode='r', encoding='gbk') as f:
        lines = f.readlines()
        lines = list(filter(lambda line: 'v' in line, lines))
        line = lines[-1]
    version = re.findall(r"v[0-9.]+", line)[0]
    return version


if __name__ == '__main__':
    filename = f"TaiYuan-Release-{get_version()}.zip"
    if os.path.exists(filename):
        os.remove(filename)
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
    for file in glob.glob('*.msu'):
        shutil.copy(file, dirname)

    # zipf = zipfile.ZipFile(filename, 'w', zipfile.ZIP_DEFLATED)
    # zipdir(dirname, zipf)
    # zipf.close()
