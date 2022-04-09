"""测试license文件"""

import shutil
def copy_record(disk):
    """复制录像配置到盘符"""
    shutil.copy("license-录像-gbk.txt", f"{disk}:/license.txt")

def copy_download(disk):
    """复制下载配置到盘符"""
    shutil.copy("license-下载-gbk.txt", f"{disk}:/license.txt")
