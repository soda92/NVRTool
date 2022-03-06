function ms22 {
    Push-Location "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build"
    cmd /c "vcvars64.bat & set" |
    ForEach-Object {
        if ($_ -match "=") {
            $v = $_.split("=", 2); Set-Item -Force -Path "ENV:\$($v[0])" -Value "$($v[1])"
        }
    }
    Pop-Location
    Write-Host "Visual Studio 2022 amd64 Command Prompt variables set." -ForegroundColor Green
}

if (Get-Variable 'foo' -Scope 'Global' -ErrorAction 'Ignore') { 
}
elseif ($args[0] -eq '--skip-verify') {
    ms22
}
else {
    ms22
    if (Test-Path build) {
        Remove-Item -Path build -Recurse -Force
    }
    cmake -B build -G "Ninja" `
        -DCMAKE_TOOLCHAIN_FILE="D:/src/vcpkg/scripts/buildsystems/vcpkg.cmake"

    if ($LastExitCode -eq 0) {
        $global:foo = 1
    }
}

if($LastExitCode -eq 0){
    cmake --build build
}