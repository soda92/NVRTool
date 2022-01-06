from get_version import get_version

if __name__ == "__main__":
    lines = []
    with open("installer.pre.nsi", mode="r", encoding="utf-8-sig") as f:
        lines = f.readlines()
    version = get_version()

    for i in range(len(lines)):
        lines[i] = lines[i].replace("{version}", version)

    with open("installer.out.nsi", mode="w", encoding="utf-8-sig") as f:
        for line in lines:
            f.write(line)
