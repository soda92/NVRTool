pwsh $PSScriptRoot/build.ps1
Push-Location $PSScriptRoot/build/src
pwsh $PSScriptRoot/src/cmake_afterbuild.ps1
Pop-Location
