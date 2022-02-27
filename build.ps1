if (Get-Variable 'foo' -Scope 'Global' -ErrorAction 'Ignore') { 
}
elseif ($args[0] -eq '--skip-verify') {
    ms22_32
}
else {
    if (Test-Path build) {
        Remove-Item -Path build -Recurse -Force
    }
    cmake -B build -G "Ninja" `
        -DCMAKE_TOOLCHAIN_FILE="D:/src/vcpkg/scripts/buildsystems/vcpkg.cmake"

    if ($LASTEXITCODE -eq 0) {
        $global:foo = 1
    }
}

cmake --build build