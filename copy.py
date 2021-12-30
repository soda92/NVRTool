import os,glob, shutil
if os.path.exists('F:'):
    for file in glob.glob("./太原*.exe"):
        print("copying ", file)
        shutil.copy(file, "F:")