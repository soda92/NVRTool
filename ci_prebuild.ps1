Set-Location C:/

if(Test-Path -Path vcpkg){
    # Remove-Item -Path vcpkg -Recurse -Force
}else{
    git clone git@github.com:microsoft/vcpkg.git
    .\bootstrap-vcpkg.bat
}


Set-Location vcpkg

.\vcpkg install fmt boost-json boost-asio boost-filesystem boost-beast --triplet x64-windows
