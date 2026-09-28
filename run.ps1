# Script auxiliar para executar o DevHub Desktop com o runtime do Qt6
$env:PATH = "C:\msys64\ucrt64\bin;" + $env:PATH
Start-Process -FilePath "e:\dds\build\DevHubCpp.exe" -WorkingDirectory "e:\dds"
Write-Host "DevHub Workspace inicializado com sucesso!" -ForegroundColor Cyan
