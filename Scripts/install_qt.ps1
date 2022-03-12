Push-Location $PSScriptRoot/../
if (Test-Path Qt) {
}
else {
    pip install -U pip
    pip install aqtinstall
    aqt list-qt windows desktop
    aqt list-qt windows desktop --arch 6.3.0
    aqt install-qt windows desktop 6.3.0 win64_msvc2019_64
}
Pop-Location
