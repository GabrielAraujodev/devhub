#include "ProcessLauncher.hpp"
#include <QProcess>
#include <QDesktopServices>
#include <QUrl>
#include <QDir>
#include <QFileInfo>

namespace devhub::infrastructure {

bool ProcessLauncher::launchIde(const QString& projectPath, const QString& ideName) {
    QString ideLower = ideName.toLower();

    if (ideLower.contains("visual studio") && !ideLower.contains("code")) {
        // Find .sln file if available
        QDir dir(projectPath);
        QStringList slnFiles = dir.entryList(QStringList() << "*.sln", QDir::Files);
        if (!slnFiles.isEmpty()) {
            QString slnPath = dir.filePath(slnFiles.first());
            return QProcess::startDetached("cmd.exe", QStringList() << "/c" << "start" << "\"\"" << slnPath);
        }
    }

    // Default or VS Code
    return QProcess::startDetached("cmd.exe", QStringList() << "/c" << "code" << projectPath);
}

bool ProcessLauncher::launchTerminal(const QString& projectPath, const QString& terminalType) {
    Q_UNUSED(terminalType);
    QString nativePath = QDir::toNativeSeparators(projectPath);
    QStringList args;
    args << "/c" << "start" << "powershell.exe" << "-NoExit" << "-Command" << QString("Set-Location -LiteralPath '%1'").arg(nativePath);
    return QProcess::startDetached("cmd.exe", args);
}

bool ProcessLauncher::launchExplorer(const QString& projectPath) {
    return QDesktopServices::openUrl(QUrl::fromLocalFile(projectPath));
}

bool ProcessLauncher::launchAiCli(const QString& projectPath, const QString& aiToolName, const QString& customCommand) {
    QString nativePath = QDir::toNativeSeparators(projectPath);
    QString tool = aiToolName.toLower().trimmed();

    if (tool.contains("cursor")) {
        // Cursor IDE
        return QProcess::startDetached("cmd.exe", QStringList() << "/c" << "cursor" << nativePath);
    }

    QString cliCommand;
    if (!customCommand.isEmpty()) {
        cliCommand = customCommand;
    } else if (tool.contains("claude")) {
        cliCommand = "claude";
    } else if (tool.contains("aider")) {
        cliCommand = "aider";
    } else if (tool.contains("antigravity") || tool.contains("gemini") || tool.contains("agy")) {
        cliCommand = "agy";
    } else if (tool.contains("ollama")) {
        cliCommand = "ollama run qwen2.5-coder";
    } else if (tool.contains("copilot")) {
        cliCommand = "gh copilot suggest";
    } else {
        cliCommand = "claude";
    }

    QStringList args;
    args << "/c" << "start" << "powershell.exe" << "-NoExit" << "-Command"
         << QString("Set-Location -LiteralPath '%1'; Write-Host '[DevHub] Ativando agente de IA: %2' -ForegroundColor Cyan; %3")
                .arg(nativePath, aiToolName, cliCommand);
    return QProcess::startDetached("cmd.exe", args);
}

QStringList ProcessLauncher::getSupportedAiTools() {
    return {"Claude Code", "Aider", "Cursor", "Antigravity (AGY)", "Ollama", "GitHub Copilot"};
}

} // namespace devhub::infrastructure
