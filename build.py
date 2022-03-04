from pathlib import Path
import os
import subprocess


def build():
    subprocess.run(["pyuic5", "CopyFile.ui", "-o", "copy_file_window.py"])
    subprocess.run(["pyuic5", "minimized_window.ui", "-o", "minimized_window.py"])


if __name__ == "__main__":
    CWD = Path(__file__).resolve().parent
    os.chdir(str(CWD))
    build()
