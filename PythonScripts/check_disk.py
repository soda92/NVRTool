import shutil
import stat
import os


def disk_exists(path):
    try:
        return stat.S_ISBLK(os.stat(path).st_mode)
    except:
        return False


def print_disk(path):
    total, used, free = shutil.disk_usage(path)

    print("Total: %d GiB" % (total // (2**30)))
    print("Used: %d GiB" % (used // (2**30)))
    print("Free: %d GiB" % (free // (2**30)))


def get_disk_size_in_gb(path):
    total, _used, _free = shutil.disk_usage(path)
    return total // (2**30)


def disk_exists(path):
    return os.path.exists(path)


def is_udisk(path):
    return disk_exists(path) and get_disk_size_in_gb(path) < 300


def is_hdd(path):
    return os.path.exists(path) and get_disk_size_in_gb(path) >= 300


def get_udisk():
    for i in ['E:', 'F:']:
        if is_udisk(i):
            return i
    raise SystemError("Cannot find udisk")


def get_hdd():
    for i in ['E:', 'F:']:
        if is_hdd(i):
            return i
    raise SystemError("Cannot find HDD")


if __name__ == "__main__":
    print(get_hdd())
    print(get_udisk())
