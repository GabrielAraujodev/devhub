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
    void onCreateProjectDialog();
    void onEditProject(const QString &projectId);
    void onScanDirectoryClicked();
    void onOpenSidePeek(const QString &projectId);
    void onCloseSidePeek();
    void onNotesChanged();
    void onLaunchIde(const QString &projectId);
    void onLaunchTerminal(const QString &projectId);
    void onLaunchExplorer(const QString &projectId);
    void onLaunchAiCli(const QString &projectId);
    void onToggleFavorite(const QString &projectId);
    void onRemoveProject(const QString &projectId);
    void onOpenSettings();
    void onToggleDarkMode();
    void onClearSearch();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    void setupUi();
    void setupCamaraUxTheme(bool isDark = false);
    void refreshTable();
    QWidget* createHeaderBar();
    QWidget* createControlBar();
    QWidget* createSidePeekPanel();
    QWidget* createEmptyStateWidget();
    void showToast(const QString &message, bool isError = false);

    infrastructure::SqliteProjectRepository m_repo;
    QList<domain::Project> m_projects;
    QString m_activeCategory{"ALL"};
    QString m_searchQuery;
    domain::Project m_selectedProject;
    bool m_isDarkMode{false};

    // UI Widgets
    QPushButton *m_btnThemeToggle{nullptr};
    QLineEdit *m_searchInput{nullptr};
    QTableWidget *m_table{nullptr};
    QWidget *m_emptyStateWidget{nullptr};
    QLabel *m_emptyTitleLabel{nullptr};
    QLabel *m_emptyDescLabel{nullptr};
    QPushButton *m_emptyActionBtn{nullptr};
    QWidget *m_sidePeekWidget{nullptr};
    QLabel *m_peekTitleLabel{nullptr};
    QLabel *m_peekPathLabel{nullptr};
    QLabel *m_peekLangLabel{nullptr};
    QLabel *m_peekBuildLabel{nullptr};
    QLabel *m_peekStdLabel{nullptr};
    QLabel *m_peekFwLabel{nullptr};
    QLabel *m_peekCatLabel{nullptr};
    QLabel *m_peekIdeLabel{nullptr};
    QLabel *m_peekAiLabel{nullptr};
    QTextEdit *m_peekNotesEdit{nullptr};
    QLabel *m_metaSummaryLabel{nullptr};
    QLabel *m_toastLabel{nullptr};
    QTimer *m_toastTimer{nullptr};
};

} // namespace devhub::ui
