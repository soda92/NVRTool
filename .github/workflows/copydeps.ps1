Set-Location $PSScriptRoot/../..
New-Item -ItemType Directory Upload

Copy-Item .\LT_WXCL_Dll\*.dll Upload
Copy-Item -Recurse -Force .\LT_WXCL_Dll\HCNetSDKCom Upload
Copy-Item -Force .\Release\*.exe Upload
Copy-Item -Force .\Release\*.dll Upload
Copy-Item -Force .\Release\*.ini Upload
