import os
import logging

logging.basicConfig(
    filename="D:/ProgramLog.txt",
    level=logging.DEBUG,
    format="%(asctime)s: %(levelname)7s: [%(name)s]: %(message)s",
    datefmt="%Y-%m-%d %H:%M:%S",
)
logging.info(f"Current directory: {os.getcwd()}")
logging.info(f"Current file: {__file__}")

if not os.path.exists("not-first-launch.txt"):
    os.system("update-profile.exe")

os.system("LT_LCWB-1A.exe")
