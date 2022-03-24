"""check for envrionments."""

import shutil
import importlib
import pathlib
import os


def test_ffmpeg():
    assert shutil.which("ffmpeg") is not None
    assert shutil.which("ffprobe") is not None


def test_packages():
    CURRENT = pathlib.Path(__file__).resolve().parent
    FILE_PATH = str(CURRENT.parent.joinpath("requirements.txt"))
    assert os.path.exists(FILE_PATH)
    dependencies = []
    with open(FILE_PATH) as f:
        dependencies = list(
            filter(
                lambda x: x != "",
                map(
                    lambda x: x.strip(),
                    filter(lambda x: not x.startswith("#"), f.readlines()),
                ),
            )
        )

    for i in dependencies:
        importlib.import_module(i)
