$a = Get-Location
$dest = "../bin"
Write-Host "copying to $a/$dest"


if (Test-Path -Path $dest) {
    Remove-Item -Path $dest -Recurse
}

New-Item -ItemType Directory -Path $dest
Copy-Item -Path ./*.exe $dest
Copy-Item -Path ./*.dll $dest

Copy-Item -Path $PSScriptRoot/../LT_WXCL_Dll/* -Destination $dest -Recurse -Force
