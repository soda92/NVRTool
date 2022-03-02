pwsh $PSScriptRoot/build.ps1
Push-Location $PSScriptRoot/build/LT_WXCL
pwsh $PSScriptRoot/LT_WXCL/cmake_afterbuild.ps1
Pop-Location
