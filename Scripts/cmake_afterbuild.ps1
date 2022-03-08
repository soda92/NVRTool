Push-Location $PSSCriptRoot/../
$dest = "bin"
Write-Host "copying to $dest"

if (Test-Path -Path $dest) {
    Remove-Item -Path $dest/* -Recurse
}

New-Item -ItemType Directory -Path $dest -Force

Copy-Item -Path build/*.exe $dest
Copy-Item -Path build/*.dll $dest

Copy-Item -Path CHANGELOG.md $dest

Copy-Item -Path HCNetSDK/lib_win64/*.dll -Destination $dest -Recurse -Force
Copy-Item -Path HCNetSDK/lib_win64/HCNetSDKCom -Destination $dest -Recurse -Force

Copy-Item -Path C:/Windows/System32/mfc140d.dll -Destination $dest

Copy-Item -Path service.py -Destination $dest
Copy-Item -Path Default.ini -Destination $dest
Copy-Item -Path 程序配置.ini -Destination $dest
