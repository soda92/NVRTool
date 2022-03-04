$a = Get-Location
$dest = "../bin"
Write-Host "copying to $a/$dest"


if (Test-Path -Path $dest) {
    Remove-Item -Path $dest/* -Recurse
}

New-Item -ItemType Directory -Path $dest -Force
Copy-Item -Path ./*.exe $dest
Copy-Item -Path ./*.dll $dest
Copy-Item -Path $PSScriptRoot/../CHANGELOG.md $dest

Copy-Item -Path D:/HCNetSDK/lib/*.dll -Destination $dest -Recurse -Force
Copy-Item -Path D:/HCNetSDK/lib/HCNetSDKCom -Destination $dest -Recurse -Force

Copy-Item -Path C:/WINDOWS/SYSTEM32/mfc140d.dll -Destination $dest

Copy-Item -Path $PSScriptRoot/../pysrc/*.py -Destination $dest
Copy-Item -Path $PSScriptRoot/../pysrc/*.ini -Destination $dest
