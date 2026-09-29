$ErrorActionPreference = "Stop"
$env:PATH = "C:\msys64\ucrt64\bin;" + $env:PATH

Write-Host "Compilando DevHub com padrão UI CamaraUX..." -ForegroundColor Cyan
cmake --build e:\dds\build

if ($LASTEXITCODE -eq 0) {
    Write-Host "`n[SUCESSO] Compilacao concluida com sucesso!" -ForegroundColor Green
} else {
    Write-Host "`n[ERRO] Falha na compilacao!" -ForegroundColor Red
}
