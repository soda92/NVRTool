if($args[0] -eq "--quick"){
    pwsh $PSScriptRoot/cmake_build.ps1 --skip-verify
}else{
    pwsh $PSScriptRoot/cmake_build.ps1
}
pwsh $PSScriptRoot/cmake_afterbuild.ps1
Pop-Location