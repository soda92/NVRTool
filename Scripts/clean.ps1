Push-Location $PSScriptRoot/../

$build_dir = "build"
if (Test-Path $build_dir) {
    Remove-Item -Path $build_dir -Recurse -Force
}

Pop-Location
