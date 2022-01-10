def read_ini(file, encoding="gbk"):
    data = []
    with open(file, encoding=encoding) as f:
        lines = f.readlines()
        lines = [line.strip() for line in lines]
        data = lines

    for i in range(len(data)):
        if "=" in data[i]:
            data[i] = data[i].split("=")
    return data


def all_keys(data):
    keys = []
    for line in data:
        if type(line) == str:
            continue
        elif type(line) == list and len(line) == 2:
            keys.append(line[0])
    return keys


def get_value(data, key):
    for line in data:
        if type(line) == str:
            continue
        elif type(line) == list and len(line) == 2:
            if line[0] == key:
                return line[1]
    raise ValueError(f"{key} not found")


def key_exists(data, key):
    exist = False
    for line in data:
        if type(line) == str:
            continue
        elif type(line) == list and len(line) == 2:
            if line[0] == key:
                exist = True
                break
    return exist


def update_key(data, key, value):
    for line in data:
        if type(line) == str:
            continue
        elif type(line) == list and len(line) == 2:
            if line[0] == key:
                line[1] = value
    return data


def write_ini(data, file, encoding="gbk"):
    f = open(file=file, encoding=encoding, mode="w")
    for line in data:
        if type(line) == str:
            f.write(line)
        elif type(line) == list and len(line) == 2:
            f.write("=".join(line))

        f.write("\n")
    f.close()


if __name__ == "__main__":
    data_from = read_ini("D:/LT_WXCL_Dll/LT_WXCLCFG.ini")
    data_to = read_ini("LT_WXCLCFG.ini")

    for key in all_keys(data_from):
        if key_exists(data_to, key):
            update_key(data_to, key, get_value(data_from, key))

    import shutil
    shutil.move("LT_WXCLCFG.ini", "LT_WXCLCFG.ini.bak")
    write_ini(data_to, "LT_WXCLCFG.ini")
    
    with open("not-first-launch.txt", mode='w') as f:
        pass
