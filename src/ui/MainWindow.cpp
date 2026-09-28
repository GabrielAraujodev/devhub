#include "MainWindow.hpp"
#include "infrastructure/ProjectInspector.hpp"
#include "infrastructure/ProcessLauncher.hpp"
#include <QHeaderView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QClipboard>
#include <QGuiApplication>
#include <QTimer>
#include <QDialog>
#include <QFormLayout>
#include <QComboBox>
#include <QIcon>
#include <QPixmap>

namespace devhub::ui {

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("DevHub Workspace");
    setWindowIcon(QIcon(":/resources/app_icon.ico"));
    resize(1280, 840);
    setMinimumSize(960, 600);

    m_repo.init();
    m_projects = m_repo.getAll();

    setupUi();
    setupUberTheme();
    refreshTable();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUberTheme() {
    setStyleSheet(R"(
        QMainWindow {
            background-color: #ffffff;
        }
        QWidget {
            font-family: 'UberMoveText', 'Inter', 'Segoe UI', sans-serif;
            color: #000000;
        }
        #topNav {
            background-color: #ffffff;
            border-bottom: 1px solid #e5e5e5;
            padding: 10px 24px;
        }
        #workspaceTitle {
            font-family: 'UberMove', 'Inter', sans-serif;
            font-size: 17px;
            font-weight: 700;
            color: #000000;
            letter-spacing: -0.2px;
        }
        #workspaceSub {
            font-size: 13px;
            color: #5e5e5e;
            font-weight: 500;
        }
        #displayTitle {
            font-family: 'UberMove', 'Inter', sans-serif;
            font-size: 32px;
            font-weight: 700;
            color: #000000;
            line-height: 40px;
        }
        #metaSummary {
            font-size: 13.5px;
            color: #5e5e5e;
            font-weight: 400;
        }
        #searchBox {
            background-color: #efefef;
            border: 1px solid transparent;
            border-radius: 999px;
            padding: 8px 20px;
            font-size: 13.5px;
            color: #000000;
        }
        #searchBox:focus {
            background-color: #ffffff;
            border: 1px solid #000000;
        }
        QPushButton#btnPrimaryPill {
            background-color: #000000;
            color: #ffffff;
            border: none;
            border-radius: 999px;
            padding: 9px 22px;
            font-size: 13px;
            font-weight: 500;
        }
        QPushButton#btnPrimaryPill:hover {
            background-color: #282828;
        }
        QPushButton#btnSecondaryPill {
            background-color: #efefef;
            color: #000000;
            border: none;
            border-radius: 999px;
            padding: 9px 20px;
            font-size: 13px;
            font-weight: 500;
        }
        QPushButton#btnSecondaryPill:hover {
            background-color: #e2e2e2;
        }
        QPushButton#filterPill {
            background-color: #efefef;
            color: #000000;
            border: none;
            border-radius: 999px;
            padding: 7px 18px;
            font-size: 12.5px;
            font-weight: 500;
        }
        QPushButton#filterPill:hover {
            background-color: #e2e2e2;
        }
        QPushButton#filterPill[active="true"] {
            background-color: #000000;
            color: #ffffff;
            font-weight: 600;
        }
        QTableWidget {
            background-color: #ffffff;
            border: 1px solid #e5e5e5;
            border-radius: 16px;
            gridline-color: #f0f0f0;
            font-size: 13px;
            selection-background-color: #f6f6f6;
            selection-color: #000000;
        }
        QHeaderView::section {
            background-color: #ffffff;
            color: #5e5e5e;
            font-size: 11px;
            font-weight: 600;
            padding: 10px 14px;
            border: none;
            border-bottom: 1px solid #e5e5e5;
            border-right: 1px solid #f0f0f0;
            text-transform: uppercase;
        }
        #sidePeekPanel {
            background-color: #ffffff;
            border-left: 1px solid #e5e5e5;
        }
        #peekTitle {
            font-family: 'UberMove', 'Inter', sans-serif;
            font-size: 24px;
            font-weight: 700;
            color: #000000;
            border: none;
            background: transparent;
        }
        #peekNotesEdit {
            background-color: #f6f6f6;
            border: 1px solid #e5e5e5;
            border-radius: 8px;
            font-size: 12.5px;
            padding: 12px;
            color: #000000;
        }
        #peekNotesEdit:focus {
            background-color: #ffffff;
            border: 1px solid #000000;
        }
        QDialog {
            background-color: #ffffff;
            color: #000000;
        }
        QComboBox {
            background-color: #efefef;
            border: 1px solid #e5e5e5;
            border-radius: 8px;
            padding: 8px 12px;
            color: #000000;
            font-size: 13px;
        }
        QComboBox QAbstractItemView {
            background-color: #ffffff;
            border: 1px solid #e5e5e5;
            color: #000000;
            selection-background-color: #efefef;
        }
    )");
}

void MainWindow::setupUi() {
    auto *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    auto *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Top Header
    mainLayout->addWidget(createHeaderBar());

    // Content Splitter
    auto *splitter = new QSplitter(Qt::Horizontal, centralWidget);
    splitter->setHandleWidth(1);

    // Left Flow: Document + Table
    auto *docContainer = new QWidget(splitter);
    auto *docLayout = new QVBoxLayout(docContainer);
    docLayout->setContentsMargins(36, 28, 36, 36);
    docLayout->setSpacing(16);

    // Headline (UberMove sentence-case)
    auto *h1 = new QLabel("Workspace de projetos", docContainer);
    h1->setObjectName("displayTitle");
    docLayout->addWidget(h1);

    m_metaSummaryLabel = new QLabel(docContainer);
    m_metaSummaryLabel->setObjectName("metaSummary");
    docLayout->addWidget(m_metaSummaryLabel);

    // Controls & Filter Tabs (Pills)
    docLayout->addWidget(createControlBar());

    // Table Widget (8 Columns with 16px border-radius container)
    m_table = new QTableWidget(docContainer);
    m_table->setColumnCount(8);
    m_table->setHorizontalHeaderLabels({"★", "Projeto", "Stack", "Build", "Padrão", "Categoria", "Caminho no disco", "Ações"});
    m_table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    m_table->setColumnWidth(0, 36);
    m_table->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Interactive);
    m_table->setColumnWidth(1, 210);
    m_table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Interactive);
    m_table->setColumnWidth(2, 90);
    m_table->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Interactive);
    m_table->setColumnWidth(3, 110);
    m_table->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Interactive);
    m_table->setColumnWidth(4, 90);
    m_table->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Interactive);
    m_table->setColumnWidth(5, 110);
    m_table->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Stretch);
    m_table->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Fixed);
    m_table->setColumnWidth(7, 230);

    m_table->verticalHeader()->setVisible(false);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setFocusPolicy(Qt::NoFocus);

    connect(m_table, &QTableWidget::cellClicked, this, &MainWindow::onTableItemClicked);

    docLayout->addWidget(m_table, 1);
    splitter->addWidget(docContainer);

    // Right Side Peek Panel (Uber Card Chrome)
    m_sidePeekWidget = createSidePeekPanel();
    m_sidePeekWidget->setVisible(false);
    splitter->addWidget(m_sidePeekWidget);

    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 0);

    mainLayout->addWidget(splitter, 1);
}

QWidget* MainWindow::createHeaderBar() {
    auto *nav = new QWidget(this);
    nav->setObjectName("topNav");
    auto *layout = new QHBoxLayout(nav);
    layout->setContentsMargins(24, 10, 24, 10);

    // Brand Label
    auto *bcLayout = new QHBoxLayout();
    bcLayout->setSpacing(10);
    auto *iconLabel = new QLabel(nav);
    QPixmap iconPix(":/resources/app_icon.png");
    if (!iconPix.isNull()) {
        iconLabel->setPixmap(iconPix.scaled(20, 20, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    auto *ws = new QLabel("DevHub", nav);
    ws->setObjectName("workspaceTitle");
    auto *sep = new QLabel("•", nav);
    sep->setObjectName("workspaceSub");
    auto *page = new QLabel("Workspace", nav);
    page->setObjectName("workspaceSub");

    if (!iconPix.isNull()) {
        bcLayout->addWidget(iconLabel);
    }
    bcLayout->addWidget(ws);
    bcLayout->addWidget(sep);
    bcLayout->addWidget(page);
    layout->addLayout(bcLayout);

    layout->addSpacing(36);

    // Search Bar (Gray Pill)
    m_searchInput = new QLineEdit(nav);
    m_searchInput->setObjectName("searchBox");
    m_searchInput->setPlaceholderText("Buscar por nome, stack (C++, Rust, Python...), tags ou caminhos (/) ...");
    m_searchInput->setFixedWidth(440);
    connect(m_searchInput, &QLineEdit::textChanged, this, &MainWindow::onSearchChanged);
    layout->addWidget(m_searchInput);

    layout->addStretch();

    // Scan Folder (Secondary Gray Pill)
    auto *btnScan = new QPushButton("Escanear pasta", nav);
    btnScan->setObjectName("btnSecondaryPill");
    connect(btnScan, &QPushButton::clicked, this, &MainWindow::onScanDirectoryClicked);
    layout->addWidget(btnScan);

    // Tools (Secondary Gray Pill)
    auto *btnSettings = new QPushButton("Ferramentas", nav);
    btnSettings->setObjectName("btnSecondaryPill");
    connect(btnSettings, &QPushButton::clicked, this, &MainWindow::onOpenSettings);
    layout->addWidget(btnSettings);

    // + Add Project (The Primary Black Pill)
    auto *btnAdd = new QPushButton("+ Novo projeto", nav);
    btnAdd->setObjectName("btnPrimaryPill");
    connect(btnAdd, &QPushButton::clicked, this, &MainWindow::onAddProjectClicked);
    layout->addWidget(btnAdd);

    return nav;
}

QWidget* MainWindow::createControlBar() {
    auto *bar = new QWidget(this);
    auto *layout = new QHBoxLayout(bar);
    layout->setContentsMargins(0, 4, 0, 4);

    auto *filterLayout = new QHBoxLayout();
    filterLayout->setSpacing(8);

    QStringList filters = {"ALL", "FAV", "C++", "Rust", "Python", "TypeScript", "Empresa", "Open Source"};
    QStringList labels  = {"Todos", "★ Favoritos", "C++", "Rust", "Python", "TypeScript", "Empresa", "Open Source"};

    for (int i = 0; i < filters.size(); ++i) {
        auto *btn = new QPushButton(labels[i], bar);
        btn->setObjectName("filterPill");
        btn->setProperty("active", filters[i] == m_activeCategory ? "true" : "false");
        QString cat = filters[i];
        connect(btn, &QPushButton::clicked, [this, cat]() {
            onCategoryClicked(cat);
        });
        filterLayout->addWidget(btn);
    }

    layout->addLayout(filterLayout);
    layout->addStretch();

    return bar;
}

QWidget* MainWindow::createSidePeekPanel() {
    auto *panel = new QWidget(this);
    panel->setObjectName("sidePeekPanel");
    panel->setFixedWidth(450);

    auto *layout = new QVBoxLayout(panel);
    layout->setContentsMargins(28, 24, 28, 24);
    layout->setSpacing(16);

    // Top action bar
    auto *topRow = new QHBoxLayout();
    auto *btnCopy = new QPushButton("Copiar caminho", panel);
    btnCopy->setObjectName("btnSecondaryPill");
    connect(btnCopy, &QPushButton::clicked, [this]() {
        QGuiApplication::clipboard()->setText(m_selectedProject.path);
        QMessageBox::information(this, "DevHub", "Caminho copiado com sucesso.");
    });
    topRow->addWidget(btnCopy);

    topRow->addStretch();

    auto *btnClose = new QPushButton("Fechar", panel);
    btnClose->setObjectName("btnSecondaryPill");
    connect(btnClose, &QPushButton::clicked, this, &MainWindow::onCloseSidePeek);
    topRow->addWidget(btnClose);

    layout->addLayout(topRow);

    m_peekTitleLabel = new QLabel(panel);
    m_peekTitleLabel->setObjectName("peekTitle");
    layout->addWidget(m_peekTitleLabel);

    // Properties Grid
    auto *propsForm = new QFormLayout();
    propsForm->setHorizontalSpacing(16);
    propsForm->setVerticalSpacing(12);

    m_peekPathLabel = new QLabel(panel);
    m_peekPathLabel->setStyleSheet("font-family: 'JetBrains Mono', Consolas, monospace; font-size: 11.5px; background: #efefef; padding: 6px 10px; border-radius: 8px; color: #000000;");
    propsForm->addRow("Caminho:", m_peekPathLabel);

    m_peekLangLabel = new QLabel(panel);
    m_peekLangLabel->setStyleSheet("font-size: 13px; font-weight: 700; color: #000000;");
    propsForm->addRow("Stack / Linguagem:", m_peekLangLabel);

    m_peekBuildLabel = new QLabel(panel);
    m_peekBuildLabel->setStyleSheet("font-size: 13px; color: #000000;");
    propsForm->addRow("Build / Toolchain:", m_peekBuildLabel);

    m_peekStdLabel = new QLabel(panel);
    m_peekStdLabel->setStyleSheet("font-size: 13px; color: #5e5e5e;");
    propsForm->addRow("Padrão / Versão:", m_peekStdLabel);

    m_peekFwLabel = new QLabel(panel);
    m_peekFwLabel->setStyleSheet("font-size: 13px; color: #5e5e5e;");
    propsForm->addRow("Frameworks:", m_peekFwLabel);

    m_peekCatLabel = new QLabel(panel);
    m_peekCatLabel->setStyleSheet("font-size: 13px; color: #000000;");
    propsForm->addRow("Categoria:", m_peekCatLabel);

    m_peekIdeLabel = new QLabel(panel);
    m_peekIdeLabel->setStyleSheet("font-size: 13px; color: #000000;");
    propsForm->addRow("IDE sugerida:", m_peekIdeLabel);

    layout->addLayout(propsForm);

    // Launch Bar (Uber Pills)
    auto *launchRow = new QHBoxLayout();
    auto *btnIde = new QPushButton("Abrir na IDE", panel);
    btnIde->setObjectName("btnPrimaryPill");
    connect(btnIde, &QPushButton::clicked, [this]() {
        onLaunchIde(m_selectedProject.id);
    });
    launchRow->addWidget(btnIde);

    auto *btnTerm = new QPushButton("Terminal", panel);
    btnTerm->setObjectName("btnSecondaryPill");
    connect(btnTerm, &QPushButton::clicked, [this]() {
        onLaunchTerminal(m_selectedProject.id);
    });
    launchRow->addWidget(btnTerm);

    auto *btnExp = new QPushButton("Explorer", panel);
    btnExp->setObjectName("btnSecondaryPill");
    connect(btnExp, &QPushButton::clicked, [this]() {
        onLaunchExplorer(m_selectedProject.id);
    });
    launchRow->addWidget(btnExp);

    layout->addLayout(launchRow);

    // Divider
    auto *divider = new QFrame(panel);
    divider->setFrameShape(QFrame::HLine);
    divider->setStyleSheet("color: #e5e5e5;");
    layout->addWidget(divider);

    // Notes Scratchpad
    auto *notesHeader = new QLabel("Anotações técnicas do projeto", panel);
    notesHeader->setStyleSheet("font-size: 12px; font-weight: 600; color: #5e5e5e; text-transform: uppercase;");
    layout->addWidget(notesHeader);

    m_peekNotesEdit = new QTextEdit(panel);
    m_peekNotesEdit->setObjectName("peekNotesEdit");
    m_peekNotesEdit->setPlaceholderText("Digite notas de compilação, variáveis de ambiente ou lembretes...");
    connect(m_peekNotesEdit, &QTextEdit::textChanged, this, &MainWindow::onNotesChanged);
    layout->addWidget(m_peekNotesEdit, 1);

    // Remove button
    auto *btnRemove = new QPushButton("Remover projeto da biblioteca", panel);
    btnRemove->setStyleSheet("background: transparent; border: none; color: #5e5e5e; font-size: 12px; text-align: left; padding: 4px 0;");
    connect(btnRemove, &QPushButton::clicked, [this]() {
        onRemoveProject(m_selectedProject.id);
    });
    layout->addWidget(btnRemove);

    return panel;
}

void MainWindow::refreshTable() {
    m_projects = m_repo.getAll();

    QList<domain::Project> filtered;
    for (const auto& p : m_projects) {
        if (m_activeCategory == "FAV" && !p.isFavorite) continue;

        if (m_activeCategory != "ALL" && m_activeCategory != "FAV") {
            bool matchesCategory = (p.category.compare(m_activeCategory, Qt::CaseInsensitive) == 0);
            bool matchesLanguage = (p.language.compare(m_activeCategory, Qt::CaseInsensitive) == 0);
            if (!matchesCategory && !matchesLanguage) continue;
        }

        if (!m_searchQuery.isEmpty()) {
            QString q = m_searchQuery.toLower();
            if (!p.name.toLower().contains(q) &&
                !p.path.toLower().contains(q) &&
                !p.language.toLower().contains(q) &&
                !p.buildSystem.toLower().contains(q) &&
                !p.cxxStandard.toLower().contains(q) &&
                !p.category.toLower().contains(q) &&
                !p.frameworks.toLower().contains(q)) {
                continue;
            }
        }
        filtered.append(p);
    }

    if (m_projects.isEmpty()) {
        m_metaSummaryLabel->setText("Nenhum repositório cadastrado · Clique em '+ Novo projeto' ou 'Escanear pasta' para adicionar seus projetos locais");
    } else {
        m_metaSummaryLabel->setText(QString("%1 projetos locais cadastrados · %2")
                                    .arg(filtered.size())
                                    .arg(m_activeCategory == "ALL" ? "Todos os projetos" : m_activeCategory));
    }

    m_table->setRowCount(filtered.size());

    for (int r = 0; r < filtered.size(); ++r) {
        const auto& p = filtered[r];

        // 0: Favorite
        auto *favItem = new QTableWidgetItem(p.isFavorite ? "★" : "☆");
        favItem->setTextAlignment(Qt::AlignCenter);
        favItem->setForeground(p.isFavorite ? QColor("#000000") : QColor("#afafaf"));
        m_table->setItem(r, 0, favItem);

        // 1: Name
        QString displayName = p.name;
        if (p.isOrphan) displayName += " ⚠️";
        auto *nameItem = new QTableWidgetItem(displayName);
        nameItem->setData(Qt::UserRole, p.id);
        nameItem->setFont(QFont("Inter", 10, QFont::DemiBold));
        nameItem->setForeground(QColor("#000000"));
        m_table->setItem(r, 1, nameItem);

        // 2: Stack
        auto *langItem = new QTableWidgetItem(p.language);
        langItem->setFont(QFont("Inter", 9, QFont::DemiBold));
        langItem->setForeground(QColor("#000000"));
        m_table->setItem(r, 2, langItem);

        // 3: Build System
        auto *buildItem = new QTableWidgetItem(p.buildSystem);
        buildItem->setFont(QFont("Inter", 9));
        buildItem->setForeground(QColor("#5e5e5e"));
        m_table->setItem(r, 3, buildItem);

        // 4: Standard / Version
        auto *stdItem = new QTableWidgetItem(p.cxxStandard);
        stdItem->setFont(QFont("Inter", 9));
        stdItem->setForeground(QColor("#5e5e5e"));
        m_table->setItem(r, 4, stdItem);

        // 5: Category
        auto *catItem = new QTableWidgetItem(p.category);
        catItem->setFont(QFont("Inter", 9));
        catItem->setForeground(QColor("#000000"));
        m_table->setItem(r, 5, catItem);

        // 6: Path
        auto *pathItem = new QTableWidgetItem(p.path);
        pathItem->setFont(QFont("JetBrains Mono", 8.5));
        pathItem->setForeground(QColor("#5e5e5e"));
        m_table->setItem(r, 6, pathItem);

        // 7: Actions Container Widget (Uber Subtle Gray Pills)
        auto *actionWidget = new QWidget();
        auto *actLayout = new QHBoxLayout(actionWidget);
        actLayout->setContentsMargins(4, 2, 4, 2);
        actLayout->setSpacing(6);

        QString pillStyle = R"(
            QPushButton {
                background-color: #efefef;
                color: #000000;
                border: none;
                border-radius: 999px;
                font-size: 11px;
                font-weight: 500;
                padding: 4px 10px;
            }
            QPushButton:hover {
                background-color: #000000;
                color: #ffffff;
            }
        )";

        auto *btnIde = new QPushButton(p.preferredIde.split(" ").first(), actionWidget);
        btnIde->setStyleSheet(pillStyle);
        QString id = p.id;
        connect(btnIde, &QPushButton::clicked, [this, id]() { onLaunchIde(id); });
        actLayout->addWidget(btnIde);

        auto *btnTerm = new QPushButton("Terminal", actionWidget);
        btnTerm->setStyleSheet(pillStyle);
        connect(btnTerm, &QPushButton::clicked, [this, id]() { onLaunchTerminal(id); });
        actLayout->addWidget(btnTerm);

        auto *btnExp = new QPushButton("Arquivos", actionWidget);
        btnExp->setStyleSheet(pillStyle);
        connect(btnExp, &QPushButton::clicked, [this, id]() { onLaunchExplorer(id); });
        actLayout->addWidget(btnExp);

        m_table->setCellWidget(r, 7, actionWidget);
        m_table->setRowHeight(r, 42);
    }
}

void MainWindow::onSearchChanged(const QString &text) {
    m_searchQuery = text.trimmed();
    refreshTable();
}

void MainWindow::onCategoryClicked(const QString &cat) {
    m_activeCategory = cat;
    auto pills = findChildren<QPushButton*>("filterPill");
    for (auto *p : pills) {
        bool match = (p->text() == "Todos" && cat == "ALL") ||
                     (p->text() == "★ Favoritos" && cat == "FAV") ||
                     (p->text() == cat);
        p->setProperty("active", match ? "true" : "false");
        p->style()->unpolish(p);
        p->style()->polish(p);
    }
    refreshTable();
}

void MainWindow::onTableItemClicked(int row, int column) {
    if (column == 0) {
        auto *item = m_table->item(row, 1);
        if (item) {
            onToggleFavorite(item->data(Qt::UserRole).toString());
        }
        return;
    }

    auto *item = m_table->item(row, 1);
    if (item) {
        onOpenSidePeek(item->data(Qt::UserRole).toString());
    }
}

void MainWindow::onOpenSidePeek(const QString &projectId) {
    for (const auto& p : m_projects) {
        if (p.id == projectId) {
            m_selectedProject = p;
            m_peekTitleLabel->setText(p.name);
            m_peekPathLabel->setText(p.path);
            m_peekLangLabel->setText(p.language);
            m_peekBuildLabel->setText(p.buildSystem);
            m_peekStdLabel->setText(p.cxxStandard);
            m_peekFwLabel->setText(p.frameworks.isEmpty() ? "Nenhum" : p.frameworks);
            m_peekCatLabel->setText(p.category);
            m_peekIdeLabel->setText(p.preferredIde);

            m_peekNotesEdit->blockSignals(true);
            m_peekNotesEdit->setPlainText(p.notes);
            m_peekNotesEdit->blockSignals(false);

            m_sidePeekWidget->setVisible(true);
            return;
        }
    }
}

void MainWindow::onCloseSidePeek() {
    m_sidePeekWidget->setVisible(false);
}

void MainWindow::onNotesChanged() {
    m_selectedProject.notes = m_peekNotesEdit->toPlainText();
    m_repo.update(m_selectedProject);
}

void MainWindow::onLaunchIde(const QString &projectId) {
    for (auto& p : m_projects) {
        if (p.id == projectId) {
            p.lastAccessed = "Agora mesmo";
            m_repo.update(p);
            infrastructure::ProcessLauncher::launchIde(p.path, p.preferredIde);
            break;
        }
    }
}

void MainWindow::onLaunchTerminal(const QString &projectId) {
    for (auto& p : m_projects) {
        if (p.id == projectId) {
            p.lastAccessed = "Agora mesmo";
            m_repo.update(p);
            infrastructure::ProcessLauncher::launchTerminal(p.path);
            break;
        }
    }
}

void MainWindow::onLaunchExplorer(const QString &projectId) {
    for (auto& p : m_projects) {
        if (p.id == projectId) {
            p.lastAccessed = "Agora mesmo";
            m_repo.update(p);
            infrastructure::ProcessLauncher::launchExplorer(p.path);
            break;
        }
    }
}

void MainWindow::onToggleFavorite(const QString &projectId) {
    for (auto& p : m_projects) {
        if (p.id == projectId) {
            p.isFavorite = !p.isFavorite;
            m_repo.update(p);
            refreshTable();
            break;
        }
    }
}

void MainWindow::onRemoveProject(const QString &projectId) {
    auto res = QMessageBox::question(this, "Remover Projeto",
                                     QString("Remover projeto da biblioteca do DevHub?\n\nOs arquivos no disco continuarão intactos."),
                                     QMessageBox::Yes | QMessageBox::No);
    if (res == QMessageBox::Yes) {
        m_repo.remove(projectId);
        onCloseSidePeek();
        refreshTable();
    }
}

void MainWindow::onAddProjectClicked() {
    QString dir = QFileDialog::getExistingDirectory(this, "Selecionar Pasta de Repositório / Projeto");
    if (dir.isEmpty()) return;

    // Check duplicate
    for (const auto& p : m_projects) {
        if (QDir::toNativeSeparators(p.path).toLower() == QDir::toNativeSeparators(dir).toLower()) {
            QMessageBox::warning(this, "DevHub", "Este diretório já está cadastrado.");
            return;
        }
    }

    auto p = infrastructure::ProjectInspector::inspect(dir);
    m_repo.save(p);
    refreshTable();
    onOpenSidePeek(p.id);
}

void MainWindow::onScanDirectoryClicked() {
    QString parentDir = QFileDialog::getExistingDirectory(this, "Selecionar Pasta para Escanear Repositórios");
    if (parentDir.isEmpty()) return;

    QDir rootDir(parentDir);
    auto subdirs = rootDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
    int addedCount = 0;

    // Check if the selected directory itself is a project
    QString rootCMake = rootDir.filePath("CMakeLists.txt");
    QString rootPkg = rootDir.filePath("package.json");
    QString rootCargo = rootDir.filePath("Cargo.toml");
    QString rootPy = rootDir.filePath("pyproject.toml");

    bool rootIsProj = QFile::exists(rootCMake) || QFile::exists(rootPkg) || QFile::exists(rootCargo) || QFile::exists(rootPy);
    if (rootIsProj) {
        bool alreadyExists = false;
        for (const auto& p : m_projects) {
            if (QDir::toNativeSeparators(p.path).toLower() == QDir::toNativeSeparators(parentDir).toLower()) {
                alreadyExists = true;
                break;
            }
        }
        if (!alreadyExists) {
            auto p = infrastructure::ProjectInspector::inspect(parentDir);
            m_repo.save(p);
            addedCount++;
        }
    }

    // Also scan immediate child directories
    for (const QString& sub : subdirs) {
        QString fullPath = rootDir.filePath(sub);
        if (QFile::exists(fullPath + "/CMakeLists.txt") ||
            QFile::exists(fullPath + "/package.json") ||
            QFile::exists(fullPath + "/Cargo.toml") ||
            QFile::exists(fullPath + "/pyproject.toml") ||
            QFile::exists(fullPath + "/requirements.txt") ||
            QFile::exists(fullPath + "/go.mod") ||
            QFile::exists(fullPath + "/.git")) {

            bool alreadyExists = false;
            for (const auto& p : m_projects) {
                if (QDir::toNativeSeparators(p.path).toLower() == QDir::toNativeSeparators(fullPath).toLower()) {
                    alreadyExists = true;
                    break;
                }
            }
            if (!alreadyExists) {
                auto p = infrastructure::ProjectInspector::inspect(fullPath);
                m_repo.save(p);
                addedCount++;
            }
        }
    }

    refreshTable();
    if (addedCount > 0) {
        QMessageBox::information(this, "DevHub", QString("%1 novos projetos reais foram descobertos e cadastrados com sucesso!").arg(addedCount));
    } else {
        QMessageBox::information(this, "DevHub", "Nenhum novo repositório com CMake, Cargo, npm, Go ou Python foi detectado nesta pasta.");
    }
}

void MainWindow::onOpenSettings() {
    QDialog dlg(this);
    dlg.setWindowTitle("Preferências de Ferramentas");
    dlg.setFixedWidth(400);

    auto *form = new QFormLayout(&dlg);
    form->setSpacing(14);

    auto *comboIde = new QComboBox(&dlg);
    comboIde->addItems({"VS Code", "Visual Studio", "CLion", "RustRover", "PyCharm", "Qt Creator"});

    auto *comboTerm = new QComboBox(&dlg);
    comboTerm->addItems({"PowerShell", "Windows Terminal", "CMD", "Git Bash"});

    form->addRow("IDE padrão:", comboIde);
    form->addRow("Terminal padrão:", comboTerm);

    auto *btnOk = new QPushButton("Salvar preferências", &dlg);
    btnOk->setObjectName("btnPrimaryPill");
    connect(btnOk, &QPushButton::clicked, &dlg, &QDialog::accept);
    form->addRow(btnOk);

    dlg.exec();
}

} // namespace devhub::ui
