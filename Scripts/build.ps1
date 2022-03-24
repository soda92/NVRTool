$a = Get-Command clss 2>&1
if($a -contains "not recognized"){
    $dir1 = "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build"
    $dir2 = "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build"
    if (Test-Path $dir1) {
        Push-Location $dir1
    }
    else {
        if (Test-Path $dir2) {
            Push-Location $dir2
        }
        else {
            throw "cannot found avaliable compiler."
        }
    }
    
    cmd /c "vcvars64.bat & set" |
    ForEach-Object {
        if ($_ -match "=") {
            $v = $_.split("=", 2); Set-Item -Force -Path "ENV:\$($v[0])" -Value "$($v[1])"
        }
    }
    Pop-Location
    Write-Host "Visual Studio 2022 amd64 Command Prompt variables set." -ForegroundColor Green
}

Push-Location $PSScriptRoot/../

cmake --build build

if ($? -eq $false){
    throw "error occored."
}

pwsh $PSScriptRoot/afterbuild.ps1

Pop-Location
