pwsh $PSScriptRoot/build.ps1 --skip-verify
Push-Location $PSScriptRoot/build/LT_WXCL
pwsh $PSScriptRoot/LT_WXCL/cmake_afterbuild.ps1
Pop-Location
