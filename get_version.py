import re

def get_version():
    line = ''
    with open("LT_WXCL/CHANGELOG.md", mode='r', encoding='gbk') as f:
        lines = f.readlines()
        lines = list(filter(lambda line: 'v' in line, lines))
        line = lines[-1]
    version = re.findall(r"v[0-9\.]+[0-9]", line)[0]
    return version
