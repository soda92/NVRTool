Push-Location C:/
if (Test-Path -Path vcpkg) {
    # Remove-Item -Path vcpkg -Recurse -Force
}
else {
    git clone git@github.com:microsoft/vcpkg.git
    .\bootstrap-vcpkg.bat
}
Pop-Location

Push-Location C:/vcpkg
.\vcpkg install fmt boost-json boost-asio boost-filesystem boost-beast `
    boost-process boost-python --triplet x64-windows
Pop-Location
