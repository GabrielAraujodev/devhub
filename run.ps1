# Script auxiliar para executar o DevHub Workspace nativo
$distExe = "e:\dds\dist\DevHub\DevHub.exe"
$buildExe = "e:\dds\build\DevHubCpp.exe"

if (Test-Path $distExe) {
    Start-Process -FilePath $distExe -WorkingDirectory "e:\dds\dist\DevHub"
    Write-Host "DevHub Workspace (Pacote Completo) aberto com sucesso!" -ForegroundColor Green
} else {
    $env:PATH = "C:\msys64\ucrt64\bin;" + $env:PATH
    $env:QT_PLUGIN_PATH = "C:\msys64\ucrt64\share\qt6\plugins"
    Start-Process -FilePath $buildExe -WorkingDirectory "e:\dds"
    Write-Host "DevHub Workspace inicializado com sucesso!" -ForegroundColor Cyan
}
