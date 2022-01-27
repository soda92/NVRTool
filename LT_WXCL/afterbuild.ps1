Set-Location $PSScriptRoot

Write-Host $args
New-Item -Type File -Force $args/log.txt | Out-Null
Write-Host "copying..."
Copy-Item CHANGELOG.md $args[0]
