#include "ProjectInspector.hpp"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QRegularExpression>
#include <QUuid>

namespace devhub::infrastructure {

domain::Project ProjectInspector::inspect(const QString& directoryPath) {
    domain::Project p;
    p.id = "proj-" + QUuid::createUuid().toString(QUuid::WithoutBraces).left(8);
    p.path = QDir::toNativeSeparators(directoryPath);

    QDir dir(directoryPath);
    if (!dir.exists()) {
        p.name = QFileInfo(directoryPath).fileName();
        p.isOrphan = true;
        p.buildSystem = "Custom";
        p.cxxStandard = "C++20";
        p.preferredIde = "VS Code";
        p.category = "Outros";
        return p;
    }

    p.name = dir.dirName();
    p.isOrphan = false;
    p.language = "C++";
    p.buildSystem = "Custom";
    p.cxxStandard = "C++20";
    p.frameworks = "Nenhum";
    p.preferredIde = "VS Code";
    p.category = "Empresa";
    p.lastAccessed = "Recém-adicionado";

    // 1. Inspect Rust (Cargo.toml)
    if (dir.exists("Cargo.toml")) {
        p.language = "Rust";
        p.buildSystem = "Cargo";
        p.cxxStandard = "Rust 2021";
        p.frameworks = "Tokio, Serde";
        p.preferredIde = "VS Code";
        return p;
    }

    // 2. Inspect Node.js / TypeScript (package.json)
    if (dir.exists("package.json")) {
        p.language = "TypeScript";
        p.buildSystem = "npm / Node";
        p.cxxStandard = "ES2024";
        p.frameworks = dir.exists("tsconfig.json") ? "TypeScript, React" : "Node.js";
        p.preferredIde = "VS Code";
        return p;
    }

    // 3. Inspect Python (pyproject.toml / requirements.txt)
    if (dir.exists("pyproject.toml") || dir.exists("requirements.txt") || dir.exists("Pipfile")) {
        p.language = "Python";
        p.buildSystem = dir.exists("pyproject.toml") ? "Poetry / Pip" : "Venv";
        p.cxxStandard = "Python 3.12";
        p.frameworks = "FastAPI, NumPy";
        p.preferredIde = "VS Code";
        return p;
    }

    // 4. Inspect Go (go.mod)
    if (dir.exists("go.mod")) {
        p.language = "Go";
        p.buildSystem = "Go Modules";
        p.cxxStandard = "Go 1.22";
        p.frameworks = "Standard Lib, Gin";
        p.preferredIde = "VS Code";
        return p;
    }

    // 5. Inspect C# (*.csproj)
    QStringList csFiles = dir.entryList(QStringList() << "*.csproj", QDir::Files);
    if (!csFiles.isEmpty()) {
        p.language = "C#";
        p.buildSystem = ".NET / MSBuild";
        p.cxxStandard = ".NET 8.0";
        p.frameworks = "ASP.NET Core";
        p.preferredIde = "Visual Studio";
        return p;
    }

    // 6. Inspect CMake (C++)
    QFile cmakeFile(dir.filePath("CMakeLists.txt"));
    if (cmakeFile.exists()) {
        p.language = "C++";
        p.buildSystem = "CMake";
        if (cmakeFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QString content = cmakeFile.readAll();
            cmakeFile.close();

            QRegularExpression rxStd("CMAKE_CXX_STANDARD\\s+(11|14|17|20|23|26)", QRegularExpression::CaseInsensitiveOption);
            auto match = rxStd.match(content);
            if (match.hasMatch()) {
                p.cxxStandard = "C++" + match.captured(1);
            }

            if (content.contains("Qt6", Qt::CaseInsensitive) || content.contains("find_package(Qt6", Qt::CaseInsensitive)) {
                p.frameworks = "Qt 6";
                p.preferredIde = "Qt Creator";
            } else if (content.contains("Qt5", Qt::CaseInsensitive) || content.contains("find_package(Qt5", Qt::CaseInsensitive)) {
                p.frameworks = "Qt 5";
                p.preferredIde = "Qt Creator";
            } else if (content.contains("vulkan", Qt::CaseInsensitive)) {
                p.frameworks = "Vulkan";
            }
        }
    } else {
        // 7. Inspect Visual Studio (.sln / .vcxproj)
        QStringList slnFiles = dir.entryList(QStringList() << "*.sln" << "*.vcxproj", QDir::Files);
        if (!slnFiles.isEmpty()) {
            p.language = "C++";
            p.buildSystem = "Visual Studio";
            p.cxxStandard = "C++17";
            p.preferredIde = "Visual Studio";
        } else if (dir.exists("build.ninja")) {
            p.language = "C++";
            p.buildSystem = "Ninja";
            p.cxxStandard = "C++20";
        }
    }

    return p;
}

} // namespace devhub::infrastructure
