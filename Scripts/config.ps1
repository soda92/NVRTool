$dir1 = "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build"
$dir2 = "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build"
if (Test-Path $dir1) {
    Push-Location $dir1
}
if (Test-Path $dir2) {
    Push-Location $dir2
}
cmd /c "vcvars64.bat & set" |
ForEach-Object {
    if ($_ -match "=") {
        $v = $_.split("=", 2); Set-Item -Force -Path "ENV:\$($v[0])" -Value "$($v[1])"
    }
}
Pop-Location
Write-Host "Visual Studio 2022 amd64 Command Prompt variables set." -ForegroundColor Green
Push-Location $PSScriptRoot/../src/qt/
$Env:Qt6_DIR = "$PSScriptRoot/../Qt/6.3.0/msvc2019_64"
$Env:Qt6WidgetsTools_DIR = "$PSScriptRoot/../Qt/6.3.0/msvc2019_64"
$Env:Qt6CoreTools_DIR = "$PSScriptRoot/../Qt/6.3.0/msvc2019_64"
$Env:Qt6GuiTools_DIR = "$PSScriptRoot/../Qt/6.3.0/msvc2019_64"

$Env:CMAKE_TOOLCHAIN_FILE = "C:/vcpkg/scripts/buildsystems/vcpkg.cmake"

cmake -B $PSScriptRoot/../build -G "Ninja" -Wno-dev -DDEBUG=1 -DNO_COPY=1
Pop-Location
