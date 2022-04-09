Push-Location $PSSCriptRoot/../
$dest = "$PSScriptRoot/../lib"


Write-Host "copying to $dest"

if (Test-Path -Path $dest) {
    Remove-Item -Path $dest/* -Recurse
}

New-Item -ItemType Directory -Path $dest -Force

Copy-Item -Path build/*.exe $dest
Copy-Item -Path build/*.dll $dest

Copy-Item -Path plugin/HCNetSDK/lib_win64/*.dll -Destination $dest -Recurse -Force
Copy-Item -Path plugin/HCNetSDK/lib_win64/HCNetSDKCom -Destination $dest -Recurse -Force

Push-Location C:/Windows/System32/
Copy-Item -Path `
    mfc140d.dll, msvcp140d.dll, VCRUNTIME140D.dll, VCRUNTIME140_1D.dll, ucrtbased.dll `
    -Destination $dest

Pop-Location

Push-Location $dest
Rename-Item stream_lib.dll stream_lib.pyd
Rename-Item serial_lib.dll serial_lib.pyd
Rename-Item tax_lib.dll tax_lib.pyd
Pop-Location

Pop-Location
