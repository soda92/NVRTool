if($args[0] -eq "--quick"){
    pwsh $PSScriptRoot/build.ps1 --skip-verify
}else{
    pwsh $PSScriptRoot/build.ps1
}
Push-Location $PSScriptRoot/build
pwsh $PSScriptRoot/cmake_afterbuild.ps1
Pop-Location
