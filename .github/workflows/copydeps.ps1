# ��ĿĿ¼
Set-Location $PSScriptRoot/../..
# �����ļ���
New-Item -ItemType Directory Upload

# ����DLL
Copy-Item .\LT_WXCL_Dll\*.dll Upload
Copy-Item -Recurse -Force .\LT_WXCL_Dll\HCNetSDKCom Upload

# ���ú�ִ�г���
Copy-Item .\LT_WXCL\*.ini Upload
Copy-Item -Force .\Release\*.exe Upload
