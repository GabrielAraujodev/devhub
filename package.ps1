param(
    [string]$AppExe = "e:\dds\build\DevHubCpp.exe",
    [string]$OutputDir = "e:\dds\dist\DevHub",
    [string]$MsysBin = "C:\msys64\ucrt64\bin",
    [string]$ZipOutput = "e:\dds\downloads\DevHub-v1.1.0-win-x64.zip",
    [string]$WebZipOutput = "e:\dds\website\downloads\DevHub-v1.1.0-win-x64.zip"
)

Write-Host "====================================================" -ForegroundColor Cyan
Write-Host "  DevHub - Gerador de Pacote de Distribuicao Windows" -ForegroundColor Cyan
Write-Host "====================================================" -ForegroundColor Cyan

# 1. Limpar e recriar pasta de saida
if (Test-Path $OutputDir) {
    Remove-Item -Recurse -Force $OutputDir
}
New-Item -ItemType Directory -Force -Path $OutputDir | Out-Null

# 2. Copiar executavel principal
Copy-Item $AppExe -Destination (Join-Path $OutputDir "DevHub.exe")
if (Test-Path "e:\dds\website\favicon.ico") {
    Copy-Item "e:\dds\website\favicon.ico" -Destination (Join-Path $OutputDir "app.ico")
}

# 3. Executar windeployqt para copiar runtime do Qt6
Write-Host "`n[1/4] Executando windeployqt..." -ForegroundColor Yellow
$windeployqt = Join-Path $MsysBin "windeployqt.exe"
& $windeployqt --no-translations --compiler-runtime --dir $OutputDir (Join-Path $OutputDir "DevHub.exe")

# 4. Resolver todas as dependencias de DLLs do UCRT64 recursivamente
Write-Host "`n[2/4] Resolvendo dependencias de DLLs do compilador/sistema..." -ForegroundColor Yellow
$objdump = Join-Path $MsysBin "objdump.exe"
$scanned = New-Object 'System.Collections.Generic.HashSet[string]' ([System.StringComparer]::OrdinalIgnoreCase)
$queue = New-Object 'System.Collections.Generic.Queue[string]'

Get-ChildItem -Path $OutputDir -Recurse -Include "*.exe","*.dll" | ForEach-Object {
    $queue.Enqueue($_.FullName)
}

$copiedCount = 0
while ($queue.Count -gt 0) {
    $currentFile = $queue.Dequeue()
    $leafName = [System.IO.Path]::GetFileName($currentFile)
    if ($scanned.Contains($leafName)) {
        continue
    }
    $scanned.Add($leafName) | Out-Null

    $output = & $objdump -p $currentFile 2>$null
    if ($output) {
        $dllMatches = $output | Select-String "DLL Name: (.*)"
        foreach ($match in $dllMatches) {
            $dll = $match.Matches[0].Groups[1].Value.Trim()
            $msysDllPath = Join-Path $MsysBin $dll
            $destPath = Join-Path $OutputDir $dll
            if ((Test-Path $msysDllPath) -and (-not (Test-Path $destPath))) {
                Copy-Item $msysDllPath -Destination $destPath
                $copiedCount++
                Write-Host "  + Copiado: $dll" -ForegroundColor Gray
                $queue.Enqueue($msysDllPath)
            }
        }
    }
}
Write-Host "Total de DLLs complementares copiadas: $copiedCount" -ForegroundColor Green

# 5. Criar scripts de conveniencia (Instalador com 1 clique e Launcher)
Write-Host "`n[3/4] Gerando scripts de instalacao e inicializacao..." -ForegroundColor Yellow

$installBatContent = @"
@echo off
setlocal
title Instalador DevHub Workspace
echo ========================================================
echo         Instalador DevHub Workspace para Windows
echo ========================================================
echo.
powershell -NoProfile -ExecutionPolicy Bypass -File "%~dp0instalar.ps1"
echo.
pause
"@
[System.IO.File]::WriteAllText((Join-Path $OutputDir "instalar.bat"), $installBatContent, [System.Text.Encoding]::UTF8)

$installPs1Content = @'
$ErrorActionPreference = "Stop"
Write-Host "Instalando DevHub no seu computador..." -ForegroundColor Cyan

$sourceDir = $PSScriptRoot
$targetDir = Join-Path $env:LOCALAPPDATA "Programs\DevHub"

# Fechar processo em execucao se houver
$existing = Get-Process -Name "DevHub" -ErrorAction SilentlyContinue
if ($existing) {
    Write-Host "Encerrando processo DevHub em execucao..." -ForegroundColor Yellow
    $existing | Stop-Process -Force
    Start-Sleep -Seconds 1
}

# Criar pasta de destino
if (-not (Test-Path $targetDir)) {
    New-Item -ItemType Directory -Force -Path $targetDir | Out-Null
}

Write-Host "Copiando arquivos para $targetDir ..." -ForegroundColor Yellow
Get-ChildItem -Path $sourceDir -Recurse | Where-Object {
    $_.FullName -notmatch "instalar\.(bat|ps1)$"
} | Copy-Item -Destination {
    $rel = $_.FullName.Substring($sourceDir.Length).TrimStart("\")
    $dest = Join-Path $targetDir $rel
    $parent = [System.IO.Path]::GetDirectoryName($dest)
    if (-not (Test-Path $parent)) { New-Item -ItemType Directory -Force -Path $parent | Out-Null }
    $dest
} -Force

$exePath = Join-Path $targetDir "DevHub.exe"
$icoPath = Join-Path $targetDir "app.ico"

# Criar atalhos (Desktop e Menu Iniciar)
$wsh = New-Object -ComObject WScript.Shell

# 1. Desktop Shortcut
$desktopPath = [System.Environment]::GetFolderPath("Desktop")
$desktopLnk = Join-Path $desktopPath "DevHub.lnk"
$shortcut = $wsh.CreateShortcut($desktopLnk)
$shortcut.TargetPath = $exePath
$shortcut.WorkingDirectory = $targetDir
$shortcut.Description = "DevHub Workspace - Painel Central de Repositorios"
if (Test-Path $icoPath) { $shortcut.IconLocation = "$icoPath,0" }
$shortcut.Save()

# 2. Start Menu Shortcut
$startMenuPath = [System.Environment]::GetFolderPath("Programs")
$startLnk = Join-Path $startMenuPath "DevHub.lnk"
$shortcutStart = $wsh.CreateShortcut($startLnk)
$shortcutStart.TargetPath = $exePath
$shortcutStart.WorkingDirectory = $targetDir
$shortcutStart.Description = "DevHub Workspace - Painel Central de Repositorios"
if (Test-Path $icoPath) { $shortcutStart.IconLocation = "$icoPath,0" }
$shortcutStart.Save()

Write-Host "`n[SUCESSO] DevHub instalado com sucesso!" -ForegroundColor Green
Write-Host " - Atalho criado na sua Area de Trabalho" -ForegroundColor White
Write-Host " - Atalho adicionado ao Menu Iniciar" -ForegroundColor White
Write-Host " - Pasta de instalacao: $targetDir" -ForegroundColor White
'@
[System.IO.File]::WriteAllText((Join-Path $OutputDir "instalar.ps1"), $installPs1Content, [System.Text.Encoding]::UTF8)

# 6. Compactar em arquivo .ZIP standalone
Write-Host "`n[4/4] Gerando arquivo ZIP standalone..." -ForegroundColor Yellow

$zipDir = [System.IO.Path]::GetDirectoryName($ZipOutput)
if (-not (Test-Path $zipDir)) { New-Item -ItemType Directory -Force -Path $zipDir | Out-Null }
if (Test-Path $ZipOutput) { 
    Remove-Item -Force $ZipOutput
    Start-Sleep -Milliseconds 500
}

$tempZip = Join-Path $zipDir "temp_build.zip"
if (Test-Path $tempZip) { Remove-Item -Force $tempZip }

Compress-Archive -Path "$OutputDir\*" -DestinationPath $tempZip -CompressionLevel Optimal
Move-Item -Path $tempZip -Destination $ZipOutput -Force

if (Test-Path $WebZipOutput) { 
    Remove-Item -Force $WebZipOutput
    Start-Sleep -Milliseconds 200
}
$webZipDir = [System.IO.Path]::GetDirectoryName($WebZipOutput)
if (-not (Test-Path $webZipDir)) { New-Item -ItemType Directory -Force -Path $webZipDir | Out-Null }
Copy-Item $ZipOutput -Destination $WebZipOutput -Force

$zipItem = Get-Item $ZipOutput
$hash = (Get-FileHash -Path $ZipOutput -Algorithm SHA256).Hash
$sizeMb = [Math]::Round($zipItem.Length / 1MB, 2)

Write-Host "`n====================================================" -ForegroundColor Green
Write-Host "  PACOTE DE DISTRIBUICAO PRONTO COM SUCESSO!" -ForegroundColor Green
Write-Host "====================================================" -ForegroundColor Green
Write-Host "Arquivo ZIP:  $ZipOutput" -ForegroundColor White
Write-Host "Tamanho:      $sizeMb MB" -ForegroundColor White
Write-Host "SHA-256:      $hash" -ForegroundColor White
Write-Host "Pasta local:  $OutputDir" -ForegroundColor White
Write-Host "====================================================" -ForegroundColor Green
