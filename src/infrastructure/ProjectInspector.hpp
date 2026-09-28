#pragma once

#include "domain/Project.hpp"
#include <QString>

namespace devhub::infrastructure {

class ProjectInspector {
public:
    static domain::Project inspect(const QString& directoryPath);
};

} // namespace devhub::infrastructure
