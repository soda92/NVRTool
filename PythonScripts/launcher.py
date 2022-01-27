import os
import logging

logging.basicConfig(
    level=logging.DEBUG,
    format="%(asctime)s: %(levelname)7s: [%(name)s]: %(message)s",
    datefmt="%Y-%m-%d %H:%M:%S",
)
logging.info(f"Current directory: {os.getcwd()}")
logging.info(f"Current file: {__file__}")


def check_hash():
    hash = ""
    with open("config-hash.txt") as f:
        hash = f.readline()
    from tasks import get_hash

    if get_hash("LT_WXCLCFG.ini") == hash:
        os.system("update-profile.exe")


if __name__ == "__main__":
    check_hash()
    os.system("LT_LCWB-1A.exe")
