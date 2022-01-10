from check_disk import get_hdd, get_udisk
import datetime


def get_config():
    file = f"{get_udisk()}/license.txt"

    lines = []
    with open(file, encoding="gbk", mode='r') as f:
        lines = f.readlines()
        lines = list(filter(lambda x: '=' in x, lines))

    lines = list(map(
        lambda x: x.split('=')[1].split('//')[0].strip()[1:-1],
        lines
    ))
    username = lines[0]
    password = lines[1]

    if username != '6A' or password != 'A6':
        raise SystemError("username or password wrong")

    channels = lines[2].split(',')
    if channels == ['00']:
        # format string: fill-align-type
        channels = [f'{str(x+1):0>2s}' for x in range(12)]

    time_start = datetime.datetime.strptime(lines[3], '%Y-%m-%d %H:%M')
    time_end = time_start+datetime.timedelta(hours=int(lines[4]))
    return channels, time_start, time_end


if __name__ == '__main__':
    print(get_config())
