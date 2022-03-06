pwsh $PSScriptRoot/ci_prebuild.ps1

if ($LastExitCode -eq 0) {
    pwsh $PSScriptRoot/cmake_build.ps1
}

if ($LastExitCode -eq 0) {
    pwsh $PSScriptRoot/cmake_afterbuild.ps1
}
