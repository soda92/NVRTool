import glob
import datetime
import os
from check_disk import get_hdd
from parse_config import get_config


def get_files():
    src_folder = f"{get_hdd()}/LT*蓝天多维"
    folder = glob.glob(src_folder)[0]

    dirnames = sorted(os.listdir(folder), reverse=False)

    channels, time_start, time_end = get_config()
    # print(channels, time_start, time_end)

    date_start = datetime.datetime(year = time_start.year, month=time_start.month, day = time_start.day)
    date_end = datetime.datetime(year = time_end.year, month=time_end.month, day = time_end.day)
    date_list = []
    if date_start > date_end:
        pass
    else:
        date_list.append(datetime.datetime.strftime(date_start, "%Y-%m-%d"))
        while date_start != date_end:
            date_start += datetime.timedelta(days=1)
            date_list.append(datetime.datetime.strftime(date_start, "%Y-%m-%d"))

    
    req_dirs = []
    for i in date_list:
        if i in dirnames:
            req_dirs.append(i)

    file_lists = []
    import pdb
    # pdb.set_trace()
    for dirname in req_dirs:
        files = []
        for file_name in os.listdir(os.path.join(folder, dirname)):
            if not file_name.endswith('mp4'):
                if file_name.endswith("idx"):
                    files.append(file_name)
                else:
                    continue
            list_ = file_name[:-4].split('_')
            channel_ = list_[2]
            time_ = list_[-2]+'_'+list_[-1]
            time_start_ = datetime.datetime.strptime(time_, "%Y%m%d_%H%M%S")
            time_end_ = time_start_ + datetime.timedelta(minutes=15)
            if (not (
                        (time_end_ < time_start) or (time_start_ > time_end))
                    ) and channel_ in channels:
                files.append(file_name)
        if len(files) != 0:
            file_lists.append([dirname, files])
    return folder, file_lists


if __name__ == '__main__':
    print(get_files())
