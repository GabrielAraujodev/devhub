#pragma once

#include <QString>
#include <QDateTime>

namespace devhub::domain {

struct Project {
    QString id;
    QString name;
    QString path;
    QString language{"C++"}; // "C++", "Rust", "Python", "TypeScript", "Go", "C#", "Polyglot"
    QString buildSystem;     // "CMake", "Cargo", "Poetry", "npm", "Ninja", "Visual Studio"
    QString cxxStandard;     // "C++20", "Rust 2021", "Python 3.12", "Node 20", etc.
    QString frameworks;      // "Qt 6", "Tokio", "FastAPI", "React", "Vulkan"
    QString category;        // "Empresa", "Open Source", "Estudos", "Outros"
    bool isFavorite{false};
    QString preferredIde;    // "VS Code", "Visual Studio", "CLion", "RustRover", "PyCharm"
    QString notes;
    QString lastAccessed;
    bool isOrphan{false};
};

} // namespace devhub::domain
