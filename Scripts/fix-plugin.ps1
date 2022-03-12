Write-Host $Env:Qt5_DIR/../../plugins/platforms/
Copy-Item $Env:Qt5_DIR/../../plugins/platforms/qwindowsd.dll build/platforms/
Remove-Item build/platforms/qwindows.dll
