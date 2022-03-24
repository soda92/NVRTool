import shutil
from delete_file import get_disk
import os, glob
import subprocess

if __name__ == "__main__":
    import sys

    if len(sys.argv) == 1:
        print("需要输入剩余空间")
        sys.exit(0)

    if len(sys.argv) == 3:
        disk = sys.argv[2] + ":"
    else:
        disk = get_disk()
    folder_list = glob.glob(os.path.join(disk, "LT-VIDEO-No000*"))
    dest_folder = ""
    if len(folder_list) == 0:
        dest_folder = os.path.join(disk, "LT-VIDEO-No0000-TEST")
        os.makedirs(dest_folder, exist_ok=True)
    else:
        dest_folder = folder_list[0]
        print("destination folder: ", dest_folder)

    # 以GiB为单位
    required_free = float(sys.argv[1]) * 2 ** 30
    total, used, free = shutil.disk_usage(disk)
    space_fill_needed = free - required_free
    test_dir = os.path.join(dest_folder, "2000-01-01")
    # if os.path.exists(test_dir):
    #     shutil.rmtree(test_dir)
    os.makedirs(test_dir, exist_ok=True)

    i = 0
    space_filled = 0
    size_per_file = 1024 * 2 ** 20  # 1GiB
    while space_filled < space_fill_needed:
        test_file = f"test_{i}.mp4"
        test_file = os.path.join(test_dir, test_file)
        if os.path.exists(test_file):
            i += 1
            continue
        print(f"creating file {test_file}...")
        # https://stackoverflow.com/a/986041/12291425
        subprocess.run(["fsutil", "file", "createnew", test_file, str(size_per_file)])
        space_filled += size_per_file
        i += 1
