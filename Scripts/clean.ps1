$build_dir = "$PSScriptRoot/../build"
if (Test-Path $build_dir) {
    Remove-Item -Path $build_dir -Recurse -Force
}
