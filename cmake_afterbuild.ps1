$dest = "$PSSCriptRoot/bin"
Write-Host "copying to $dest"

if (Test-Path -Path $dest) {
    Remove-Item -Path $dest/* -Recurse
}

New-Item -ItemType Directory -Path $dest -Force
Copy-Item -Path ./*.exe $dest
Copy-Item -Path ./*.dll $dest
Copy-Item -Path $PSScriptRoot/CHANGELOG.md $dest

Copy-Item -Path $PSScriptRoot/HCNetSDK/lib/*.dll -Destination $dest -Recurse -Force
Copy-Item -Path $PSScriptRoot/HCNetSDK/lib/HCNetSDKCom -Destination $dest -Recurse -Force

Copy-Item -Path C:/WINDOWS/SYSTEM32/mfc140d.dll -Destination $dest

Copy-Item -Path $PSScriptRoot/service.py -Destination $dest
Copy-Item -Path $PSScriptRoot/Default.ini -Destination $dest
Copy-Item -Path $PSScriptRoot/程序配置.ini -Destination $dest
