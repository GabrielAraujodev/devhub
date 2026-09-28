#pragma once

#include <QString>

namespace devhub::infrastructure {

class ProcessLauncher {
public:
    static bool launchIde(const QString& projectPath, const QString& ideName);
    static bool launchTerminal(const QString& projectPath, const QString& terminalType = "PowerShell");
    static bool launchExplorer(const QString& projectPath);
};

} // namespace devhub::infrastructure
