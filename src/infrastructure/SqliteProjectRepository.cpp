#include "SqliteProjectRepository.hpp"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardPaths>
#include <QDir>
#include <QFileInfo>

namespace devhub::infrastructure {

SqliteProjectRepository::SqliteProjectRepository() {}

SqliteProjectRepository::~SqliteProjectRepository() {}

bool SqliteProjectRepository::init() {
    QString appDataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(appDataDir);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    QString dbPath = dir.filePath("devhub.db");

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath);

    if (!db.open()) {
        return false;
    }

    QSqlQuery q;
    q.exec("CREATE TABLE IF NOT EXISTS projects ("
           "id TEXT PRIMARY KEY, "
           "name TEXT NOT NULL, "
           "path TEXT NOT NULL UNIQUE, "
           "language TEXT DEFAULT 'C++', "
           "build_system TEXT, "
           "cxx_standard TEXT, "
           "frameworks TEXT, "
           "category TEXT, "
           "is_favorite INTEGER DEFAULT 0, "
           "preferred_ide TEXT, "
           "notes TEXT, "
           "last_accessed TEXT"
           ");");

    // Migration in case table already existed without language
    q.exec("ALTER TABLE projects ADD COLUMN language TEXT DEFAULT 'C++';");
    q.exec("ALTER TABLE projects ADD COLUMN preferred_ai_tool TEXT DEFAULT 'Claude Code';");

    // Purge any legacy mock seeds
    q.exec("DELETE FROM projects WHERE id IN ('proj-1', 'proj-2', 'proj-3', 'proj-4', 'proj-5', 'proj-6');");

    return true;
}

QList<domain::Project> SqliteProjectRepository::getAll() {
    QList<domain::Project> list;
    QSqlQuery q("SELECT id, name, path, language, build_system, cxx_standard, frameworks, category, is_favorite, preferred_ide, notes, last_accessed, preferred_ai_tool FROM projects ORDER BY is_favorite DESC, name ASC;");
    while (q.next()) {
        domain::Project p;
        p.id = q.value(0).toString();
        p.name = q.value(1).toString();
        p.path = q.value(2).toString();
        p.language = q.value(3).toString();
        p.buildSystem = q.value(4).toString();
        p.cxxStandard = q.value(5).toString();
        p.frameworks = q.value(6).toString();
        p.category = q.value(7).toString();
        p.isFavorite = (q.value(8).toInt() == 1);
        p.preferredIde = q.value(9).toString();
        p.notes = q.value(10).toString();
        p.lastAccessed = q.value(11).toString();
        p.preferredAiTool = q.value(12).toString();
        if (p.preferredAiTool.isEmpty()) {
            p.preferredAiTool = "Claude Code";
        }
        p.isOrphan = !QFileInfo::exists(p.path);
        list.append(p);
    }
    return list;
}

bool SqliteProjectRepository::save(const domain::Project& p) {
    QSqlQuery q;
    q.prepare("INSERT OR REPLACE INTO projects (id, name, path, language, build_system, cxx_standard, frameworks, category, is_favorite, preferred_ide, notes, last_accessed, preferred_ai_tool) "
              "VALUES (:id, :name, :path, :lang, :build, :std, :fw, :cat, :fav, :ide, :notes, :last, :ai);");
    q.bindValue(":id", p.id);
    q.bindValue(":name", p.name);
    q.bindValue(":path", p.path);
    q.bindValue(":lang", p.language.isEmpty() ? "C++" : p.language);
    q.bindValue(":build", p.buildSystem);
    q.bindValue(":std", p.cxxStandard);
    q.bindValue(":fw", p.frameworks);
    q.bindValue(":cat", p.category);
    q.bindValue(":fav", p.isFavorite ? 1 : 0);
    q.bindValue(":ide", p.preferredIde);
    q.bindValue(":notes", p.notes);
    q.bindValue(":last", p.lastAccessed);
    q.bindValue(":ai", p.preferredAiTool.isEmpty() ? "Claude Code" : p.preferredAiTool);
    return q.exec();
}

bool SqliteProjectRepository::update(const domain::Project& p) {
    return save(p);
}

bool SqliteProjectRepository::remove(const QString& id) {
    QSqlQuery q;
    q.prepare("DELETE FROM projects WHERE id = :id;");
    q.bindValue(":id", id);
    return q.exec();
}

} // namespace devhub::infrastructure
