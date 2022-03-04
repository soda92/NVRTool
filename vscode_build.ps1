if($args[0] -eq "--quick"){
    pwsh $PSScriptRoot/build.ps1 --skip-verify
}else{
    pwsh $PSScriptRoot/build.ps1
}
Push-Location $PSScriptRoot/build/src
pwsh $PSScriptRoot/src/cmake_afterbuild.ps1
Pop-Location
