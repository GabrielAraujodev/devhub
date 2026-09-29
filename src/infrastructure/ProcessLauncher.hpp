#pragma once

#include <QString>
#include <QStringList>

namespace devhub::infrastructure {

class ProcessLauncher {
public:
    static bool launchIde(const QString& projectPath, const QString& ideName);
    static bool launchTerminal(const QString& projectPath, const QString& terminalType = "PowerShell");
    static bool launchExplorer(const QString& projectPath);
    static bool launchAiCli(const QString& projectPath, const QString& aiToolName, const QString& customCommand = "");
    static QStringList getSupportedAiTools();
};

} // namespace devhub::infrastructure
