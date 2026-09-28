#pragma once

#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QPushButton>
#include <QSplitter>
#include <QWidget>
#include <QVBoxLayout>
#include "domain/Project.hpp"
#include "infrastructure/SqliteProjectRepository.hpp"

namespace devhub::ui {

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onSearchChanged(const QString &text);
    void onCategoryClicked(const QString &cat);
    void onTableItemClicked(int row, int column);
    void onAddProjectClicked();
    void onScanDirectoryClicked();
    void onOpenSidePeek(const QString &projectId);
    void onCloseSidePeek();
    void onNotesChanged();
    void onLaunchIde(const QString &projectId);
    void onLaunchTerminal(const QString &projectId);
    void onLaunchExplorer(const QString &projectId);
    void onToggleFavorite(const QString &projectId);
    void onRemoveProject(const QString &projectId);
    void onOpenSettings();

private:
    void setupUi();
    void setupUberTheme();
    void refreshTable();
    QWidget* createHeaderBar();
    QWidget* createControlBar();
    QWidget* createSidePeekPanel();

    infrastructure::SqliteProjectRepository m_repo;
    QList<domain::Project> m_projects;
    QString m_activeCategory{"ALL"};
    QString m_searchQuery;
    domain::Project m_selectedProject;

    // UI Widgets
    QLineEdit *m_searchInput{nullptr};
    QTableWidget *m_table{nullptr};
    QWidget *m_sidePeekWidget{nullptr};
    QLabel *m_peekTitleLabel{nullptr};
    QLabel *m_peekPathLabel{nullptr};
    QLabel *m_peekLangLabel{nullptr};
    QLabel *m_peekBuildLabel{nullptr};
    QLabel *m_peekStdLabel{nullptr};
    QLabel *m_peekFwLabel{nullptr};
    QLabel *m_peekCatLabel{nullptr};
    QLabel *m_peekIdeLabel{nullptr};
    QTextEdit *m_peekNotesEdit{nullptr};
    QLabel *m_metaSummaryLabel{nullptr};
};

} // namespace devhub::ui
