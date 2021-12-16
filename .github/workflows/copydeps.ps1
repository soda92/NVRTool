# 项目目录
Set-Location $PSScriptRoot/../..
# 创建文件夹
New-Item -ItemType Directory Upload

# 复制DLL
Copy-Item .\LT_WXCL_Dll\*.dll Upload
Copy-Item -Recurse -Force .\LT_WXCL_Dll\HCNetSDKCom Upload

# 配置和执行程序
Copy-Item .\LT_WXCL\*.ini Upload
Copy-Item -Force .\Release\*.exe Upload
