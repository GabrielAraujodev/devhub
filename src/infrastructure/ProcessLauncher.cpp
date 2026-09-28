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

} // namespace devhub::infrastructure
