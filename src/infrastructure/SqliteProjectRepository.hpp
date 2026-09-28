#pragma once

#include "domain/Project.hpp"
#include <QList>
#include <QString>

namespace devhub::infrastructure {

class SqliteProjectRepository {
public:
    SqliteProjectRepository();
    ~SqliteProjectRepository();

    bool init();
    QList<domain::Project> getAll();
    bool save(const domain::Project& project);
    bool update(const domain::Project& project);
    bool remove(const QString& id);
};

} // namespace devhub::infrastructure
