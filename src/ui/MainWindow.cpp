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
#include <QCheckBox>
#include <QFile>
#include <QTextStream>
#include <QStandardPaths>
#include <QDateTime>
#include <QIcon>
#include <QPixmap>
#include <QSettings>

namespace devhub::ui {

static QString getDialogStylesheet(bool isDark) {
    if (isDark) {
        return R"(
            QDialog {
                background-color: #0a0a0a;
                color: #ffffff;
            }
            QLabel {
                color: #ffffff;
            }
            QLabel#dlgTitle {
                font-size: 16px;
                font-weight: 700;
                color: #ffffff;
            }
            QLabel#dlgSub {
                font-size: 13px;
                color: #a3a3a3;
                margin-bottom: 6px;
            }
            QLineEdit, QComboBox, QTextEdit {
                background-color: #171717;
                border: 1px solid #2e2e2e;
                border-radius: 6px;
                padding: 7px 12px;
                font-size: 13px;
                color: #ffffff;
            }
            QLineEdit:focus, QComboBox:focus, QTextEdit:focus {
                border: 2px solid #ffffff;
                background-color: #000000;
            }
            QComboBox QAbstractItemView {
                background-color: #0a0a0a;
                border: 1px solid #2e2e2e;
                color: #ffffff;
                selection-background-color: #262626;
                selection-color: #ffffff;
            }
            QCheckBox {
                font-size: 13px;
                color: #e5e5e5;
                spacing: 8px;
            }
            QPushButton#btnBrowse {
                background-color: #171717;
                border: 1px solid #2e2e2e;
                border-radius: 6px;
                padding: 7px 14px;
                font-size: 12.5px;
                color: #ffffff;
            }
            QPushButton#btnBrowse:hover {
                background-color: #262626;
            }
            QPushButton#btnLaunch, QPushButton#btnSave, QPushButton#btnCreate, QPushButton#btnPrimaryPill {
                background-color: #ffffff;
                color: #000000;
                font-weight: 600;
                padding: 8px 22px;
                border-radius: 6px;
                border: none;
            }
            QPushButton#btnLaunch:hover, QPushButton#btnSave:hover, QPushButton#btnCreate:hover, QPushButton#btnPrimaryPill:hover {
                background-color: #e5e5e5;
            }
            QPushButton#btnCancel {
                background-color: #171717;
                color: #a3a3a3;
                border: 1px solid #2e2e2e;
                padding: 8px 16px;
                border-radius: 6px;
            }
            QPushButton#btnCancel:hover {
                background-color: #262626;
                color: #ffffff;
            }
        )";
    } else {
        return R"(
            QDialog {
                background-color: #f8fafc;
                color: #0f172a;
            }
            QLabel {
                color: #0f172a;
            }
            QLabel#dlgTitle {
                font-size: 16px;
                font-weight: 700;
                color: #0f172a;
            }
            QLabel#dlgSub {
                font-size: 13px;
                color: #475569;
                margin-bottom: 6px;
            }
            QLineEdit, QComboBox, QTextEdit {
                background-color: #ffffff;
                border: 1px solid #cbd5e1;
                border-radius: 6px;
                padding: 7px 12px;
                font-size: 13px;
                color: #0f172a;
            }
            QLineEdit:focus, QComboBox:focus, QTextEdit:focus {
                border: 2px solid #0f172a;
            }
            QComboBox QAbstractItemView {
                background-color: #ffffff;
                border: 1px solid #cbd5e1;
                color: #0f172a;
                selection-background-color: #f1f5f9;
                selection-color: #0f172a;
            }
            QCheckBox {
                font-size: 13px;
                color: #1e293b;
                spacing: 8px;
            }
            QPushButton#btnBrowse {
                background-color: #f1f5f9;
                border: 1px solid #cbd5e1;
                border-radius: 6px;
                padding: 7px 14px;
                font-size: 12.5px;
                color: #0f172a;
            }
            QPushButton#btnBrowse:hover {
                background-color: #e2e8f0;
            }
            QPushButton#btnLaunch, QPushButton#btnSave, QPushButton#btnCreate, QPushButton#btnPrimaryPill {
                background-color: #0f172a;
                color: #ffffff;
                font-weight: 600;
                padding: 8px 22px;
                border-radius: 6px;
                border: none;
            }
            QPushButton#btnLaunch:hover, QPushButton#btnSave:hover, QPushButton#btnCreate:hover, QPushButton#btnPrimaryPill:hover {
                background-color: #1e293b;
            }
            QPushButton#btnCancel {
                background-color: #f1f5f9;
                color: #0f172a;
                border: 1px solid #cbd5e1;
                padding: 8px 16px;
                border-radius: 6px;
            }
            QPushButton#btnCancel:hover {
                background-color: #e2e8f0;
            }
        )";
    }
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("DevHub Workspace");
    setWindowIcon(QIcon(":/resources/app_icon.ico"));
    resize(1280, 840);
    setMinimumSize(960, 600);

    QSettings settings("DevHub", "DevHub");
    m_isDarkMode = settings.value("darkMode", false).toBool();

    m_repo.init();
    m_projects = m_repo.getAll();

    setupUi();
    setupCamaraUxTheme(m_isDarkMode);
    refreshTable();
}

MainWindow::~MainWindow() {}

void MainWindow::setupCamaraUxTheme(bool isDark) {
    if (isDark) {
        setStyleSheet(R"(
        /* ==========================================================
           CAMARAUX DESIGN SYSTEM - TRUE BLACK MONOCHROME TOKENS
           Canvas: Pure Black (#000000), Surface: Dark Carbon (#0a0a0a)
           Inputs & Secondary: Neutral Dark (#171717), Borders: (#262626)
           Primary Action: High-Contrast Pure White Pill (#ffffff)
           ========================================================== */

        QMainWindow {
            background-color: #000000;
        }
        QWidget {
            font-family: 'Inter', 'Segoe UI', -apple-system, sans-serif;
            color: #ffffff;
        }

        /* Top Navigation Header */
        #topNav {
            background-color: #0a0a0a;
            border-bottom: 1px solid #262626;
            padding: 10px 24px;
        }
        #workspaceTitle {
            font-size: 16px;
            font-weight: 700;
            color: #ffffff;
            letter-spacing: -0.2px;
        }
        #workspaceSub {
            font-size: 13px;
            color: #a3a3a3;
            font-weight: 500;
        }

        /* Typography */
        #displayTitle {
            font-size: 28px;
            font-weight: 700;
            color: #ffffff;
            letter-spacing: -0.4px;
        }
        #metaSummary {
            font-size: 13.5px;
            color: #a3a3a3;
            font-weight: 400;
        }

        /* Search Input */
        #searchBox {
            background-color: #171717;
            border: 1px solid #2e2e2e;
            border-radius: 999px;
            padding: 8px 18px;
            font-size: 13px;
            color: #ffffff;
        }
        #searchBox:focus {
            background-color: #000000;
            border: 2px solid #ffffff;
        }

        /* Primary Button (CamaraUX Solid White Action on Dark) */
        QPushButton#btnPrimaryPill {
            background-color: #ffffff;
            color: #000000;
            border: 1px solid #ffffff;
            border-radius: 999px;
            padding: 8px 20px;
            font-size: 13px;
            font-weight: 600;
        }
        QPushButton#btnPrimaryPill:hover {
            background-color: #e5e5e5;
            border-color: #e5e5e5;
        }
        QPushButton#btnPrimaryPill:pressed {
            background-color: #d4d4d4;
        }

        /* Secondary Button (Neutral Surface) */
        QPushButton#btnSecondaryPill, QPushButton#btnThemeToggle {
            background-color: #171717;
            color: #ffffff;
            border: 1px solid #2e2e2e;
            border-radius: 999px;
            padding: 8px 18px;
            font-size: 13px;
            font-weight: 500;
        }
        QPushButton#btnSecondaryPill:hover, QPushButton#btnThemeToggle:hover {
            background-color: #262626;
            border-color: #404040;
        }
        QPushButton#btnSecondaryPill:pressed, QPushButton#btnThemeToggle:pressed {
            background-color: #0a0a0a;
        }

        /* AI Agent Launcher (Monochrome Tech Pill) */
        QPushButton#btnAiAgent {
            background-color: #171717;
            color: #ffffff;
            border: 1px solid #525252;
            border-radius: 999px;
            padding: 8px 18px;
            font-size: 13px;
            font-weight: 600;
        }
        QPushButton#btnAiAgent:hover {
            background-color: #262626;
            border-color: #ffffff;
            color: #ffffff;
        }
        QPushButton#btnAiAgent:pressed {
            background-color: #0a0a0a;
        }

        /* Destructive Button (CamaraUX Error Semantic) */
        QPushButton#btnDestructive {
            background-color: #450a0a;
            color: #f87171;
            border: 1px solid #7f1d1d;
            border-radius: 999px;
            padding: 8px 18px;
            font-size: 12.5px;
            font-weight: 600;
            text-align: center;
        }
        QPushButton#btnDestructive:hover {
            background-color: #dc2626;
            color: #ffffff;
            border-color: #dc2626;
        }

        /* Filter Pills (CamaraUX Filter Bar Pattern) */
        QPushButton#filterPill {
            background-color: #0a0a0a;
            color: #a3a3a3;
            border: 1px solid #262626;
            border-radius: 999px;
            padding: 6px 16px;
            font-size: 12.5px;
            font-weight: 500;
        }
        QPushButton#filterPill:hover {
            background-color: #171717;
            color: #ffffff;
            border-color: #404040;
        }
        QPushButton#filterPill[active="true"] {
            background-color: #ffffff;
            color: #000000;
            border: 1px solid #ffffff;
            font-weight: 600;
        }

        /* Table & Headers */
        QTableWidget {
            background-color: #0a0a0a;
            border: 1px solid #262626;
            border-radius: 12px;
            gridline-color: #171717;
            font-size: 13px;
            color: #ffffff;
            selection-background-color: #1f1f1f;
            selection-color: #ffffff;
        }
        QHeaderView::section {
            background-color: #000000;
            color: #a3a3a3;
            font-size: 11px;
            font-weight: 600;
            padding: 10px 14px;
            border: none;
            border-bottom: 1px solid #262626;
            border-right: 1px solid #171717;
            text-transform: uppercase;
            letter-spacing: 0.5px;
        }

        /* Side Peek Drawer */
        #sidePeekPanel {
            background-color: #0a0a0a;
            border-left: 1px solid #262626;
        }
        #peekTitle {
            font-size: 22px;
            font-weight: 700;
            color: #ffffff;
            border: none;
            background: transparent;
        }
        #peekPathLabel {
            font-family: 'JetBrains Mono', Consolas, monospace;
            font-size: 11.5px;
            background-color: #171717;
            padding: 6px 10px;
            border-radius: 8px;
            color: #ffffff;
            border: 1px solid #2e2e2e;
        }
        #peekLangLabel, #peekBuildLabel, #peekCatLabel, #peekIdeLabel {
            font-size: 13px;
            font-weight: 600;
            color: #ffffff;
        }
        #peekStdLabel, #peekFwLabel {
            font-size: 13px;
            color: #a3a3a3;
        }
        #peekAiLabel {
            font-size: 13px;
            color: #ffffff;
            font-weight: 600;
        }
        #peekDivider {
            color: #262626;
            background-color: #262626;
            max-height: 1px;
        }
        #notesHeader {
            font-size: 12px;
            font-weight: 600;
            color: #a3a3a3;
            text-transform: uppercase;
            letter-spacing: 0.5px;
        }
        #peekNotesEdit {
            background-color: #000000;
            border: 1px solid #262626;
            border-radius: 8px;
            font-size: 12.5px;
            padding: 12px;
            color: #ffffff;
        }
        #peekNotesEdit:focus {
            background-color: #0a0a0a;
            border: 2px solid #ffffff;
        }

        /* Empty State Container */
        #emptyStateContainer {
            background-color: #0a0a0a;
            border: 1px dashed #2e2e2e;
            border-radius: 12px;
        }
        #emptyTitleLabel {
            font-size: 17px;
            font-weight: 700;
            color: #ffffff;
        }
        #emptyDescLabel {
            font-size: 13.5px;
            color: #a3a3a3;
        }

        /* Scrollbars */
        QScrollBar:vertical {
            background: #000000;
            width: 10px;
            border-radius: 5px;
            margin: 0px;
        }
        QScrollBar::handle:vertical {
            background: #262626;
            min-height: 20px;
            border-radius: 5px;
        }
        QScrollBar::handle:vertical:hover {
            background: #404040;
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;
        }
        )");
    } else {
        setStyleSheet(R"(
        /* ==========================================================
           CAMARAUX DESIGN SYSTEM - LIGHT MODE TOKENS
           ========================================================== */

        QMainWindow {
            background-color: #f8fafc; /* slate-50 */
        }
        QWidget {
            font-family: 'Inter', 'Segoe UI', -apple-system, sans-serif;
            color: #0f172a; /* slate-900 */
        }

        /* Top Navigation Header */
        #topNav {
            background-color: #ffffff;
            border-bottom: 1px solid #e2e8f0; /* slate-200 */
            padding: 10px 24px;
        }
        #workspaceTitle {
            font-size: 16px;
            font-weight: 700;
            color: #0f172a;
            letter-spacing: -0.2px;
        }
        #workspaceSub {
            font-size: 13px;
            color: #64748b; /* slate-500 */
            font-weight: 500;
        }

        /* Typography */
        #displayTitle {
            font-size: 28px;
            font-weight: 700;
            color: #0f172a;
            letter-spacing: -0.4px;
        }
        #metaSummary {
            font-size: 13.5px;
            color: #64748b;
            font-weight: 400;
        }

        /* Search Input */
        #searchBox {
            background-color: #f1f5f9; /* slate-100 */
            border: 1px solid #e2e8f0;
            border-radius: 999px;
            padding: 8px 18px;
            font-size: 13px;
            color: #0f172a;
        }
        #searchBox:focus {
            background-color: #ffffff;
            border: 2px solid #0f172a;
        }

        /* Primary Button (CamaraUX Dark Slate Action) */
        QPushButton#btnPrimaryPill {
            background-color: #0f172a;
            color: #ffffff;
            border: 1px solid #0f172a;
            border-radius: 999px;
            padding: 8px 20px;
            font-size: 13px;
            font-weight: 600;
        }
        QPushButton#btnPrimaryPill:hover {
            background-color: #1e293b;
            border-color: #1e293b;
        }
        QPushButton#btnPrimaryPill:pressed {
            background-color: #334155;
        }

        /* Secondary Button (Neutral Surface) */
        QPushButton#btnSecondaryPill, QPushButton#btnThemeToggle {
            background-color: #ffffff;
            color: #0f172a;
            border: 1px solid #cbd5e1;
            border-radius: 999px;
            padding: 8px 18px;
            font-size: 13px;
            font-weight: 500;
        }
        QPushButton#btnSecondaryPill:hover, QPushButton#btnThemeToggle:hover {
            background-color: #f1f5f9;
            border-color: #94a3b8;
        }
        QPushButton#btnSecondaryPill:pressed, QPushButton#btnThemeToggle:pressed {
            background-color: #e2e8f0;
        }

        /* AI Agent Launcher (CamaraUX Slate with Sky Accent) */
        QPushButton#btnAiAgent {
            background-color: #0f172a;
            color: #ffffff;
            border: 1px solid #1e293b;
            border-radius: 999px;
            padding: 8px 18px;
            font-size: 13px;
            font-weight: 600;
        }
        QPushButton#btnAiAgent:hover {
            background-color: #1e293b;
            color: #38bdf8;
            border-color: #38bdf8;
        }
        QPushButton#btnAiAgent:pressed {
            background-color: #0f172a;
        }

        /* Destructive Button (CamaraUX Error Semantic) */
        QPushButton#btnDestructive {
            background-color: #fef2f2;
            color: #dc2626;
            border: 1px solid #fecaca;
            border-radius: 999px;
            padding: 8px 18px;
            font-size: 12.5px;
            font-weight: 600;
            text-align: center;
        }
        QPushButton#btnDestructive:hover {
            background-color: #dc2626;
            color: #ffffff;
            border-color: #dc2626;
        }

        /* Filter Pills (CamaraUX Filter Bar Pattern) */
        QPushButton#filterPill {
            background-color: #ffffff;
            color: #475569;
            border: 1px solid #e2e8f0;
            border-radius: 999px;
            padding: 6px 16px;
            font-size: 12.5px;
            font-weight: 500;
        }
        QPushButton#filterPill:hover {
            background-color: #f1f5f9;
            color: #0f172a;
            border-color: #cbd5e1;
        }
        QPushButton#filterPill[active="true"] {
            background-color: #0f172a;
            color: #ffffff;
            border: 1px solid #0f172a;
            font-weight: 600;
        }

        /* Table & Headers */
        QTableWidget {
            background-color: #ffffff;
            border: 1px solid #e2e8f0;
            border-radius: 12px;
            gridline-color: #f8fafc;
            font-size: 13px;
            selection-background-color: #f1f5f9;
            selection-color: #0f172a;
        }
        QHeaderView::section {
            background-color: #f8fafc;
            color: #64748b;
            font-size: 11px;
            font-weight: 600;
            padding: 10px 14px;
            border: none;
            border-bottom: 1px solid #e2e8f0;
            border-right: 1px solid #f1f5f9;
            text-transform: uppercase;
            letter-spacing: 0.5px;
        }

        /* Side Peek Drawer */
        #sidePeekPanel {
            background-color: #ffffff;
            border-left: 1px solid #e2e8f0;
        }
        #peekTitle {
            font-size: 22px;
            font-weight: 700;
            color: #0f172a;
            border: none;
            background: transparent;
        }
        #peekPathLabel {
            font-family: 'JetBrains Mono', Consolas, monospace;
            font-size: 11.5px;
            background-color: #f1f5f9;
            padding: 6px 10px;
            border-radius: 8px;
            color: #0f172a;
            border: 1px solid #e2e8f0;
        }
        #peekLangLabel, #peekBuildLabel, #peekCatLabel, #peekIdeLabel {
            font-size: 13px;
            font-weight: 700;
            color: #0f172a;
        }
        #peekStdLabel, #peekFwLabel {
            font-size: 13px;
            color: #64748b;
        }
        #peekAiLabel {
            font-size: 13px;
            color: #0f172a;
            font-weight: 600;
        }
        #peekDivider {
            color: #e2e8f0;
            background-color: #e2e8f0;
            max-height: 1px;
        }
        #notesHeader {
            font-size: 12px;
            font-weight: 600;
            color: #64748b;
            text-transform: uppercase;
            letter-spacing: 0.5px;
        }
        #peekNotesEdit {
            background-color: #f8fafc;
            border: 1px solid #e2e8f0;
            border-radius: 8px;
            font-size: 12.5px;
            padding: 12px;
            color: #0f172a;
        }
        #peekNotesEdit:focus {
            background-color: #ffffff;
            border: 2px solid #0f172a;
        }

        /* Empty State Container */
        #emptyStateContainer {
            background-color: #ffffff;
            border: 1px dashed #cbd5e1;
            border-radius: 12px;
        }
        #emptyTitleLabel {
            font-size: 17px;
            font-weight: 700;
            color: #0f172a;
        }
        #emptyDescLabel {
            font-size: 13.5px;
            color: #64748b;
        }

        /* Scrollbars */
        QScrollBar:vertical {
            background: #f8fafc;
            width: 10px;
            border-radius: 5px;
            margin: 0px;
        }
        QScrollBar::handle:vertical {
            background: #cbd5e1;
            min-height: 20px;
            border-radius: 5px;
        }
        QScrollBar::handle:vertical:hover {
            background: #94a3b8;
        }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical {
            height: 0px;
        }
        )");
    }
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

    m_emptyStateWidget = createEmptyStateWidget();
    docLayout->addWidget(m_emptyStateWidget, 1);

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

    // Theme Toggle (Pill)
    m_btnThemeToggle = new QPushButton(m_isDarkMode ? "☀️ Claro" : "🌙 Escuro", nav);
    m_btnThemeToggle->setObjectName("btnThemeToggle");
    m_btnThemeToggle->setToolTip("Alternar entre Modo Claro e Modo Escuro (CamaraUX Slate Tokens)");
    connect(m_btnThemeToggle, &QPushButton::clicked, this, &MainWindow::onToggleDarkMode);
    layout->addWidget(m_btnThemeToggle);

    // + Add Project (The Primary Black Pill)
    auto *btnAdd = new QPushButton("+ Novo projeto", nav);
    btnAdd->setObjectName("btnPrimaryPill");
    connect(btnAdd, &QPushButton::clicked, this, &MainWindow::onCreateProjectDialog);
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
    connect(btnCopy, &QPushButton::clicked, [this, btnCopy]() {
        QGuiApplication::clipboard()->setText(m_selectedProject.path);
        QString oldText = btnCopy->text();
        btnCopy->setText("✓ Copiado!");
        showToast("✓ Caminho copiado para a área de transferência");
        QTimer::singleShot(2000, [btnCopy, oldText]() {
            btnCopy->setText(oldText);
        });
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
    m_peekPathLabel->setObjectName("peekPathLabel");
    propsForm->addRow("Caminho:", m_peekPathLabel);

    m_peekLangLabel = new QLabel(panel);
    m_peekLangLabel->setObjectName("peekLangLabel");
    propsForm->addRow("Stack / Linguagem:", m_peekLangLabel);

    m_peekBuildLabel = new QLabel(panel);
    m_peekBuildLabel->setObjectName("peekBuildLabel");
    propsForm->addRow("Build / Toolchain:", m_peekBuildLabel);

    m_peekStdLabel = new QLabel(panel);
    m_peekStdLabel->setObjectName("peekStdLabel");
    propsForm->addRow("Padrão / Versão:", m_peekStdLabel);

    m_peekFwLabel = new QLabel(panel);
    m_peekFwLabel->setObjectName("peekFwLabel");
    propsForm->addRow("Frameworks:", m_peekFwLabel);

    m_peekCatLabel = new QLabel(panel);
    m_peekCatLabel->setObjectName("peekCatLabel");
    propsForm->addRow("Categoria:", m_peekCatLabel);

    m_peekIdeLabel = new QLabel(panel);
    m_peekIdeLabel->setObjectName("peekIdeLabel");
    propsForm->addRow("IDE sugerida:", m_peekIdeLabel);

    m_peekAiLabel = new QLabel(panel);
    m_peekAiLabel->setObjectName("peekAiLabel");
    propsForm->addRow("Agente IA:", m_peekAiLabel);

    layout->addLayout(propsForm);

    // Launch Bar (Primary & Secondary Actions)
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

    // AI Agent CLI Launcher Button (Claude Code, Aider, Cursor, Antigravity, Ollama)
    auto *btnAi = new QPushButton("🤖 Lançar Agente IA", panel);
    btnAi->setObjectName("btnAiAgent");
    btnAi->setToolTip("Lançar Claude Code, Aider, Cursor, Antigravity ou Ollama neste repositório");
    connect(btnAi, &QPushButton::clicked, [this]() {
        onLaunchAiCli(m_selectedProject.id);
    });
    layout->addWidget(btnAi);

    // Divider
    auto *divider = new QFrame(panel);
    divider->setObjectName("peekDivider");
    divider->setFrameShape(QFrame::HLine);
    layout->addWidget(divider);

    // Notes Scratchpad
    auto *notesHeader = new QLabel("Anotações técnicas do projeto", panel);
    notesHeader->setObjectName("notesHeader");
    layout->addWidget(notesHeader);

    m_peekNotesEdit = new QTextEdit(panel);
    m_peekNotesEdit->setObjectName("peekNotesEdit");
    m_peekNotesEdit->setPlaceholderText("Digite notas de compilação, variáveis de ambiente ou lembretes...");
    connect(m_peekNotesEdit, &QTextEdit::textChanged, this, &MainWindow::onNotesChanged);
    layout->addWidget(m_peekNotesEdit, 1);

    // Management Row (Edit & Remove)
    auto *mgmtRow = new QHBoxLayout();
    auto *btnEdit = new QPushButton("✏️ Editar", panel);
    btnEdit->setObjectName("btnSecondaryPill");
    btnEdit->setToolTip("Editar metadados, stack e ferramentas do projeto");
    connect(btnEdit, &QPushButton::clicked, [this]() {
        onEditProject(m_selectedProject.id);
    });
    mgmtRow->addWidget(btnEdit);

    auto *btnRemove = new QPushButton("Desvincular / Excluir", panel);
    btnRemove->setObjectName("btnDestructive");
    btnRemove->setToolTip("Desvincular da biblioteca ou excluir pasta do disco");
    connect(btnRemove, &QPushButton::clicked, [this]() {
        onRemoveProject(m_selectedProject.id);
    });
    mgmtRow->addWidget(btnRemove);

    layout->addLayout(mgmtRow);

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

    // CamaraUX Empty State Pattern (10865 & 10703)
    if (filtered.isEmpty()) {
        m_table->setVisible(false);
        if (m_emptyStateWidget) {
            m_emptyStateWidget->setVisible(true);
            if (m_projects.isEmpty()) {
                m_emptyTitleLabel->setText("Sua biblioteca está vazia");
                m_emptyDescLabel->setText("Adicione seus repositórios em C++, Rust, Python, Go ou TypeScript para organizar e orquestrar suas ferramentas locais.");
                m_emptyActionBtn->setText("+ Novo projeto");
                disconnect(m_emptyActionBtn, nullptr, nullptr, nullptr);
                connect(m_emptyActionBtn, &QPushButton::clicked, this, &MainWindow::onCreateProjectDialog);
            } else {
                m_emptyTitleLabel->setText(m_searchQuery.isEmpty() ? "Nenhum projeto nesta categoria" : QString("Nenhum resultado para \"%1\"").arg(m_searchQuery));
                m_emptyDescLabel->setText("Tente ajustar o termo de pesquisa ou limpar os filtros ativos para visualizar seus projetos.");
                m_emptyActionBtn->setText("Limpar busca e filtros");
                disconnect(m_emptyActionBtn, nullptr, nullptr, nullptr);
                connect(m_emptyActionBtn, &QPushButton::clicked, this, &MainWindow::onClearSearch);
            }
        }
        return;
    }

    m_table->setVisible(true);
    if (m_emptyStateWidget) {
        m_emptyStateWidget->setVisible(false);
    }

    m_table->setRowCount(filtered.size());

    for (int r = 0; r < filtered.size(); ++r) {
        const auto& p = filtered[r];

        // 0: Favorite
        auto *favItem = new QTableWidgetItem(p.isFavorite ? "★" : "☆");
        favItem->setTextAlignment(Qt::AlignCenter);
        if (m_isDarkMode) {
            favItem->setForeground(p.isFavorite ? QColor("#ffffff") : QColor("#525252"));
        } else {
            favItem->setForeground(p.isFavorite ? QColor("#0f172a") : QColor("#94a3b8"));
        }
        m_table->setItem(r, 0, favItem);

        // 1: Name
        QString displayName = p.name;
        if (p.isOrphan) displayName += " ⚠️";
        auto *nameItem = new QTableWidgetItem(displayName);
        nameItem->setData(Qt::UserRole, p.id);
        nameItem->setFont(QFont("Inter", 10, QFont::DemiBold));
        nameItem->setForeground(m_isDarkMode ? QColor("#ffffff") : QColor("#0f172a"));
        m_table->setItem(r, 1, nameItem);

        // 2: Stack Badge (CamaraUX High-Contrast Badges)
        auto *langItem = new QTableWidgetItem(p.language);
        langItem->setTextAlignment(Qt::AlignCenter);
        langItem->setFont(QFont("Inter", 8.5, QFont::DemiBold));

        if (m_isDarkMode) {
            if (p.language == "C++") {
                langItem->setBackground(QColor("#1f1f23"));
                langItem->setForeground(QColor("#f4f4f5"));
            } else if (p.language == "Rust") {
                langItem->setBackground(QColor("#271e1b"));
                langItem->setForeground(QColor("#fdba74"));
            } else if (p.language == "Python") {
                langItem->setBackground(QColor("#272218"));
                langItem->setForeground(QColor("#fde047"));
            } else if (p.language == "TypeScript" || p.language == "JavaScript") {
                langItem->setBackground(QColor("#1a1f26"));
                langItem->setForeground(QColor("#e2e8f0"));
            } else if (p.language == "Go") {
                langItem->setBackground(QColor("#182523"));
                langItem->setForeground(QColor("#a7f3d0"));
            } else if (p.language == "C#") {
                langItem->setBackground(QColor("#23182b"));
                langItem->setForeground(QColor("#f5d0fe"));
            } else {
                langItem->setBackground(QColor("#1f1f23"));
                langItem->setForeground(QColor("#d4d4d8"));
            }
        } else {
            if (p.language == "C++") {
                langItem->setBackground(QColor("#e0e7ff")); // indigo-100
                langItem->setForeground(QColor("#3730a3")); // indigo-800
            } else if (p.language == "Rust") {
                langItem->setBackground(QColor("#ffedd5")); // orange-100
                langItem->setForeground(QColor("#9a3412")); // orange-800
            } else if (p.language == "Python") {
                langItem->setBackground(QColor("#fef3c7")); // amber-100
                langItem->setForeground(QColor("#92400e")); // amber-800
            } else if (p.language == "TypeScript" || p.language == "JavaScript") {
                langItem->setBackground(QColor("#e0f2fe")); // sky-100
                langItem->setForeground(QColor("#075985")); // sky-800
            } else if (p.language == "Go") {
                langItem->setBackground(QColor("#ccfbf1")); // teal-100
                langItem->setForeground(QColor("#115e59")); // teal-800
            } else if (p.language == "C#") {
                langItem->setBackground(QColor("#f3e8ff")); // purple-100
                langItem->setForeground(QColor("#6b21a8")); // purple-800
            } else {
                langItem->setBackground(QColor("#f1f5f9"));
                langItem->setForeground(QColor("#334155"));
            }
        }
        m_table->setItem(r, 2, langItem);

        // 3: Build System
        auto *buildItem = new QTableWidgetItem(p.buildSystem);
        buildItem->setFont(QFont("Inter", 9));
        buildItem->setForeground(m_isDarkMode ? QColor("#a3a3a3") : QColor("#475569"));
        m_table->setItem(r, 3, buildItem);

        // 4: Standard / Version
        auto *stdItem = new QTableWidgetItem(p.cxxStandard);
        stdItem->setFont(QFont("Inter", 9));
        stdItem->setForeground(m_isDarkMode ? QColor("#737373") : QColor("#64748b"));
        m_table->setItem(r, 4, stdItem);

        // 5: Category
        auto *catItem = new QTableWidgetItem(p.category);
        catItem->setFont(QFont("Inter", 9));
        catItem->setForeground(m_isDarkMode ? QColor("#e5e5e5") : QColor("#0f172a"));
        m_table->setItem(r, 5, catItem);

        // 6: Path
        auto *pathItem = new QTableWidgetItem(p.path);
        pathItem->setFont(QFont("JetBrains Mono", 8.5));
        pathItem->setForeground(m_isDarkMode ? QColor("#737373") : QColor("#64748b"));
        m_table->setItem(r, 6, pathItem);

        // 7: Actions Container Widget (CamaraUX Neutral Pill Actions)
        auto *actionWidget = new QWidget();
        auto *actLayout = new QHBoxLayout(actionWidget);
        actLayout->setContentsMargins(4, 2, 4, 2);
        actLayout->setSpacing(6);

        QString pillStyle = m_isDarkMode ? R"(
            QPushButton {
                background-color: #171717;
                color: #ffffff;
                border: 1px solid #2e2e2e;
                border-radius: 999px;
                font-size: 11px;
                font-weight: 500;
                padding: 4px 10px;
            }
            QPushButton:hover {
                background-color: #ffffff;
                color: #000000;
                border-color: #ffffff;
            }
        )" : R"(
            QPushButton {
                background-color: #f1f5f9;
                color: #0f172a;
                border: 1px solid #e2e8f0;
                border-radius: 999px;
                font-size: 11px;
                font-weight: 500;
                padding: 4px 10px;
            }
            QPushButton:hover {
                background-color: #0f172a;
                color: #ffffff;
                border-color: #0f172a;
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
            m_peekAiLabel->setText(p.preferredAiTool.isEmpty() ? "Claude Code" : p.preferredAiTool);

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

void MainWindow::onLaunchAiCli(const QString &projectId) {
    domain::Project* target = nullptr;
    for (auto& p : m_projects) {
        if (p.id == projectId) {
            target = &p;
            break;
        }
    }
    if (!target) return;

    QDialog dlg(this);
    dlg.setWindowTitle("Lançar Agente de IA — DevHub");
    dlg.setMinimumWidth(440);
    dlg.setStyleSheet(getDialogStylesheet(m_isDarkMode));

    auto *vbox = new QVBoxLayout(&dlg);
    vbox->setSpacing(12);
    vbox->setContentsMargins(24, 24, 24, 24);

    auto *titleLbl = new QLabel("🤖 Lançar Agente de IA no Terminal", &dlg);
    titleLbl->setObjectName("dlgTitle");
    vbox->addWidget(titleLbl);

    auto *subLbl = new QLabel(QString("Selecione o agente de IA para ativar no repositório <b>%1</b>:").arg(target->name), &dlg);
    subLbl->setObjectName("dlgSub");
    vbox->addWidget(subLbl);

    auto *form = new QFormLayout();
    form->setSpacing(10);

    auto *aiCombo = new QComboBox(&dlg);
    aiCombo->addItem("Claude Code (Anthropic)", "Claude Code");
    aiCombo->addItem("Aider (Git AI Pair Programming)", "Aider");
    aiCombo->addItem("Cursor IDE", "Cursor");
    aiCombo->addItem("Antigravity / Gemini CLI (Google)", "Antigravity (AGY)");
    aiCombo->addItem("Ollama (Modelos Locais Qwen/Llama)", "Ollama");
    aiCombo->addItem("GitHub Copilot CLI", "GitHub Copilot");
    aiCombo->addItem("Comando Personalizado...", "Custom");

    int idx = aiCombo->findData(target->preferredAiTool);
    if (idx >= 0) {
        aiCombo->setCurrentIndex(idx);
    }
    form->addRow("Agente IA:", aiCombo);

    auto *customCmdEdit = new QLineEdit(&dlg);
    customCmdEdit->setPlaceholderText("Ex: claude, aider, uvx aider, cursor ., agy");
    customCmdEdit->setVisible(aiCombo->currentData().toString() == "Custom");
    form->addRow("Comando / CLI:", customCmdEdit);

    QObject::connect(aiCombo, &QComboBox::currentIndexChanged, [customCmdEdit, aiCombo]() {
        customCmdEdit->setVisible(aiCombo->currentData().toString() == "Custom");
    });

    vbox->addLayout(form);

    auto *btnRow = new QHBoxLayout();
    btnRow->addStretch();

    auto *btnCancel = new QPushButton("Cancelar", &dlg);
    btnCancel->setObjectName("btnCancel");
    QObject::connect(btnCancel, &QPushButton::clicked, &dlg, &QDialog::reject);
    btnRow->addWidget(btnCancel);

    auto *btnLaunch = new QPushButton("Lançar Agente", &dlg);
    btnLaunch->setObjectName("btnLaunch");
    btnLaunch->setDefault(true);
    QObject::connect(btnLaunch, &QPushButton::clicked, &dlg, &QDialog::accept);
    btnRow->addWidget(btnLaunch);

    vbox->addLayout(btnRow);

    if (dlg.exec() == QDialog::Accepted) {
        QString chosenTool = aiCombo->currentData().toString();
        QString customCmd = (chosenTool == "Custom") ? customCmdEdit->text().trimmed() : "";

        target->preferredAiTool = chosenTool;
        target->lastAccessed = "Agora mesmo";
        m_repo.update(*target);
        m_peekAiLabel->setText(chosenTool);

        infrastructure::ProcessLauncher::launchAiCli(target->path, chosenTool, customCmd);
        showToast(QString("Iniciando %1 em %2...").arg(chosenTool, target->name));
    }
}

void MainWindow::onToggleFavorite(const QString &projectId) {
    for (auto& p : m_projects) {
        if (p.id == projectId) {
            p.isFavorite = !p.isFavorite;
            m_repo.update(p);
            refreshTable();
            showToast(p.isFavorite ? "★ Adicionado aos favoritos" : "☆ Removido dos favoritos");
            break;
        }
    }
}

// CamaraUX Destructive Action Pattern (11789-como-tratar-acoes-destrutivas)
void MainWindow::onRemoveProject(const QString &projectId) {
    domain::Project* target = nullptr;
    for (auto& p : m_projects) {
        if (p.id == projectId) {
            target = &p;
            break;
        }
    }
    if (!target) return;

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Gerenciar Exclusão de Projeto");
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setText(QString("Como deseja proceder com o projeto \"%1\"?").arg(target->name));
    msgBox.setInformativeText("• Desvincular: Remove apenas o registro no DevHub. Seus arquivos em \"" + target->path + "\" continuam 100% seguros e intactos no SSD.\n\n• Excluir pasta do disco: Remove permanentemente todos os arquivos e códigos da pasta física.");

    auto *btnUnlink = msgBox.addButton("Desvincular do DevHub", QMessageBox::AcceptRole);
    auto *btnDeleteDisk = msgBox.addButton("Excluir pasta do disco...", QMessageBox::DestructiveRole);
    auto *btnCancel = msgBox.addButton("Cancelar", QMessageBox::RejectRole);
    msgBox.setDefaultButton(btnCancel);

    msgBox.exec();

    if (msgBox.clickedButton() == btnUnlink) {
        m_repo.remove(projectId);
        onCloseSidePeek();
        refreshTable();
        showToast(QString("Projeto \"%1\" desvinculado da biblioteca.").arg(target->name));
    } else if (msgBox.clickedButton() == btnDeleteDisk) {
        // Confirmação crítica de segurança adicional
        QMessageBox confirmBox(this);
        confirmBox.setWindowTitle("⚠️ Confirmação Crítica de Exclusão");
        confirmBox.setIcon(QMessageBox::Critical);
        confirmBox.setText(QString("TEM CERTEZA ABSOLUTA que deseja apagar a pasta física no disco?"));
        confirmBox.setInformativeText("Diretório a ser DESTRUÍDO permanentemente do SSD:\n" + target->path + "\n\nEsta operação é IRREVERSÍVEL.");

        auto *btnYesDelete = confirmBox.addButton("Sim, apagar pasta permanentemente", QMessageBox::DestructiveRole);
        auto *btnNoCancel = confirmBox.addButton("Cancelar", QMessageBox::RejectRole);
        confirmBox.setDefaultButton(btnNoCancel);

        confirmBox.exec();

        if (confirmBox.clickedButton() == btnYesDelete) {
            QString pathToDelete = target->path;
            QDir dir(pathToDelete);
            bool removed = dir.removeRecursively();

            m_repo.remove(projectId);
            onCloseSidePeek();
            refreshTable();

            if (removed) {
                showToast(QString("Pasta física e projeto excluídos do disco."), false);
            } else {
                showToast(QString("Registro removido, mas alguns arquivos não puderam ser apagados."), true);
            }
        }
    }
}

void MainWindow::onEditProject(const QString &projectId) {
    domain::Project* target = nullptr;
    for (auto& p : m_projects) {
        if (p.id == projectId) {
            target = &p;
            break;
        }
    }
    if (!target) return;

    QDialog dlg(this);
    dlg.setWindowTitle(QString("Editar Projeto — %1").arg(target->name));
    dlg.setMinimumWidth(500);
    dlg.setStyleSheet(getDialogStylesheet(m_isDarkMode));

    auto *vbox = new QVBoxLayout(&dlg);
    vbox->setSpacing(12);
    vbox->setContentsMargins(24, 24, 24, 24);

    auto *titleLbl = new QLabel(QString("✏️ Editar Metadados — %1").arg(target->name), &dlg);
    titleLbl->setObjectName("dlgTitle");
    vbox->addWidget(titleLbl);

    auto *subLbl = new QLabel("Atualize o nome, categoria, ferramentas recomendadas e informações do repositório.", &dlg);
    subLbl->setObjectName("dlgSub");
    vbox->addWidget(subLbl);

    auto *form = new QFormLayout();
    form->setSpacing(10);

    auto *nameEdit = new QLineEdit(target->name, &dlg);
    form->addRow("Nome de exibição:", nameEdit);

    auto *langCombo = new QComboBox(&dlg);
    langCombo->addItems({"C++", "Rust", "Python", "Go", "TypeScript", "C#", "Polyglot", "Outro"});
    langCombo->setCurrentText(target->language);
    form->addRow("Stack / Linguagem:", langCombo);

    auto *stdEdit = new QLineEdit(target->cxxStandard, &dlg);
    form->addRow("Padrão / Versão:", stdEdit);

    auto *buildEdit = new QLineEdit(target->buildSystem, &dlg);
    form->addRow("Sistema de Build:", buildEdit);

    auto *fwEdit = new QLineEdit(target->frameworks, &dlg);
    form->addRow("Frameworks:", fwEdit);

    auto *catCombo = new QComboBox(&dlg);
    catCombo->addItems({"Empresa", "Open Source", "Estudos", "Outros"});
    catCombo->setCurrentText(target->category);
    form->addRow("Categoria:", catCombo);

    auto *ideCombo = new QComboBox(&dlg);
    ideCombo->addItems({"VS Code", "Visual Studio", "Qt Creator", "RustRover", "PyCharm", "CLion"});
    ideCombo->setCurrentText(target->preferredIde);
    form->addRow("IDE sugerida:", ideCombo);

    auto *aiCombo = new QComboBox(&dlg);
    aiCombo->addItems({"Claude Code", "Aider", "Cursor", "Antigravity (AGY)", "Ollama", "GitHub Copilot"});
    aiCombo->setCurrentText(target->preferredAiTool.isEmpty() ? "Claude Code" : target->preferredAiTool);
    form->addRow("Agente IA preferido:", aiCombo);

    auto *pathEdit = new QLineEdit(target->path, &dlg);
    form->addRow("Caminho no disco:", pathEdit);

    vbox->addLayout(form);

    auto *btnRow = new QHBoxLayout();
    btnRow->addStretch();

    auto *btnCancel = new QPushButton("Cancelar", &dlg);
    btnCancel->setObjectName("btnCancel");
    connect(btnCancel, &QPushButton::clicked, &dlg, &QDialog::reject);
    btnRow->addWidget(btnCancel);

    auto *btnSave = new QPushButton("Salvar Alterações", &dlg);
    btnSave->setObjectName("btnSave");
    btnSave->setDefault(true);
    connect(btnSave, &QPushButton::clicked, &dlg, &QDialog::accept);
    btnRow->addWidget(btnSave);

    vbox->addLayout(btnRow);

    if (dlg.exec() == QDialog::Accepted) {
        target->name = nameEdit->text().trimmed();
        target->language = langCombo->currentText();
        target->cxxStandard = stdEdit->text().trimmed();
        target->buildSystem = buildEdit->text().trimmed();
        target->frameworks = fwEdit->text().trimmed();
        target->category = catCombo->currentText();
        target->preferredIde = ideCombo->currentText();
        target->preferredAiTool = aiCombo->currentText();
        target->path = QDir::toNativeSeparators(pathEdit->text().trimmed());

        m_repo.update(*target);
        refreshTable();
        onOpenSidePeek(target->id);
        showToast(QString("✓ Metadados de \"%1\" atualizados!").arg(target->name));
    }
}

void MainWindow::onCreateProjectDialog() {
    QDialog dlg(this);
    dlg.setWindowTitle("Criar Novo Projeto & Diretório — DevHub");
    dlg.setMinimumWidth(540);
    dlg.setStyleSheet(getDialogStylesheet(m_isDarkMode));

    auto *vbox = new QVBoxLayout(&dlg);
    vbox->setSpacing(14);
    vbox->setContentsMargins(24, 24, 24, 24);

    auto *titleLbl = new QLabel("📁 Criar Novo Projeto & Diretório no Disco", &dlg);
    titleLbl->setObjectName("dlgTitle");
    vbox->addWidget(titleLbl);

    auto *subLbl = new QLabel("Crie uma nova pasta com scaffolding inicial ou vincule um repositório existente.", &dlg);
    subLbl->setObjectName("dlgSub");
    vbox->addWidget(subLbl);

    // Mode Selector
    auto *modeBox = new QHBoxLayout();
    auto *modeCombo = new QComboBox(&dlg);
    modeCombo->addItem("📁 Criar nova pasta e estrutura (Scaffolding)", "NEW");
    modeCombo->addItem("🔗 Vincular pasta existente no disco", "EXISTING");
    modeBox->addWidget(new QLabel("Modo:", &dlg));
    modeBox->addWidget(modeCombo, 1);
    vbox->addLayout(modeBox);

    auto *form = new QFormLayout();
    form->setSpacing(10);

    // Project Name
    auto *nameEdit = new QLineEdit(&dlg);
    nameEdit->setPlaceholderText("ex: meu-novo-servico");
    form->addRow("Nome do projeto:", nameEdit);

    // Parent Folder
    auto *folderLayout = new QHBoxLayout();
    auto *folderEdit = new QLineEdit(&dlg);
    QString defaultParent = "E:\\workspace";
    if (!QDir(defaultParent).exists()) {
        defaultParent = QDir::currentPath();
    }
    folderEdit->setText(defaultParent);
    auto *btnBrowse = new QPushButton("Procurar...", &dlg);
    btnBrowse->setObjectName("btnBrowse");
    folderLayout->addWidget(folderEdit, 1);
    folderLayout->addWidget(btnBrowse);
    form->addRow("Pasta de destino:", folderLayout);

    // Resulting Path Label
    auto *pathPreview = new QLabel(&dlg);
    pathPreview->setStyleSheet("font-size: 11.5px; color: #64748b; font-family: monospace;");
    form->addRow("Caminho final:", pathPreview);

    auto updatePathPreview = [nameEdit, folderEdit, pathPreview]() {
        QString n = nameEdit->text().trimmed();
        QString f = folderEdit->text().trimmed();
        if (n.isEmpty()) {
            pathPreview->setText(f.isEmpty() ? "-" : f);
        } else {
            pathPreview->setText(QDir::toNativeSeparators(QDir(f).filePath(n)));
        }
    };
    updatePathPreview();
    connect(nameEdit, &QLineEdit::textChanged, updatePathPreview);
    connect(folderEdit, &QLineEdit::textChanged, updatePathPreview);

    connect(btnBrowse, &QPushButton::clicked, [&dlg, folderEdit]() {
        QString dir = QFileDialog::getExistingDirectory(&dlg, "Selecionar Pasta Pai", folderEdit->text());
        if (!dir.isEmpty()) {
            folderEdit->setText(dir);
        }
    });

    // Language / Stack
    auto *langCombo = new QComboBox(&dlg);
    langCombo->addItem("C++", "C++");
    langCombo->addItem("Rust", "Rust");
    langCombo->addItem("Python", "Python");
    langCombo->addItem("Go", "Go");
    langCombo->addItem("TypeScript", "TypeScript");
    langCombo->addItem("C#", "C#");
    langCombo->addItem("Outro", "Outro");
    form->addRow("Stack / Linguagem:", langCombo);

    // Language Standard
    auto *stdEdit = new QLineEdit(&dlg);
    stdEdit->setText("C++20");
    form->addRow("Padrão / Versão:", stdEdit);

    // Build System
    auto *buildEdit = new QLineEdit(&dlg);
    buildEdit->setText("CMake");
    form->addRow("Sistema de build:", buildEdit);

    // Category
    auto *catCombo = new QComboBox(&dlg);
    catCombo->addItem("Empresa");
    catCombo->addItem("Open Source");
    catCombo->addItem("Estudos");
    catCombo->addItem("Outros");
    form->addRow("Categoria:", catCombo);

    // Preferred IDE
    auto *ideCombo = new QComboBox(&dlg);
    ideCombo->addItem("VS Code");
    ideCombo->addItem("Visual Studio");
    ideCombo->addItem("Qt Creator");
    ideCombo->addItem("RustRover");
    ideCombo->addItem("PyCharm");
    ideCombo->addItem("CLion");
    form->addRow("IDE sugerida:", ideCombo);

    // Preferred AI Agent
    auto *aiCombo = new QComboBox(&dlg);
    aiCombo->addItem("Claude Code");
    aiCombo->addItem("Aider");
    aiCombo->addItem("Cursor");
    aiCombo->addItem("Antigravity (AGY)");
    aiCombo->addItem("Ollama");
    form->addRow("Agente IA preferido:", aiCombo);

    // Scaffolding Checkbox
    auto *scaffoldCheck = new QCheckBox("Criar arquivos base da stack no disco (CMakeLists.txt, main, README, .gitignore)", &dlg);
    scaffoldCheck->setChecked(true);
    form->addRow("", scaffoldCheck);

    // Update defaults when stack changes
    connect(langCombo, &QComboBox::currentIndexChanged, [langCombo, stdEdit, buildEdit, ideCombo, scaffoldCheck]() {
        QString lang = langCombo->currentText();
        if (lang == "C++") {
            stdEdit->setText("C++20");
            buildEdit->setText("CMake");
            ideCombo->setCurrentText("VS Code");
            scaffoldCheck->setText("Criar CMakeLists.txt, main.cpp, .gitignore e README.md");
        } else if (lang == "Rust") {
            stdEdit->setText("Rust 2021");
            buildEdit->setText("Cargo");
            ideCombo->setCurrentText("VS Code");
            scaffoldCheck->setText("Criar Cargo.toml, src/main.rs, .gitignore e README.md");
        } else if (lang == "Python") {
            stdEdit->setText("Python 3.12");
            buildEdit->setText("Poetry");
            ideCombo->setCurrentText("VS Code");
            scaffoldCheck->setText("Criar pyproject.toml, main.py, .gitignore e README.md");
        } else if (lang == "Go") {
            stdEdit->setText("Go 1.22");
            buildEdit->setText("go.mod");
            ideCombo->setCurrentText("VS Code");
            scaffoldCheck->setText("Criar go.mod, main.go, .gitignore e README.md");
        } else if (lang == "TypeScript") {
            stdEdit->setText("ES2024");
            buildEdit->setText("npm / Vite");
            ideCombo->setCurrentText("VS Code");
            scaffoldCheck->setText("Criar package.json, src/index.ts, .gitignore e README.md");
        } else if (lang == "C#") {
            stdEdit->setText(".NET 8.0");
            buildEdit->setText("dotnet");
            ideCombo->setCurrentText("Visual Studio");
            scaffoldCheck->setText("Criar Program.cs, .gitignore e README.md");
        }
    });

    // Toggle mode
    connect(modeCombo, &QComboBox::currentIndexChanged, [modeCombo, scaffoldCheck, nameEdit]() {
        bool isNew = (modeCombo->currentData().toString() == "NEW");
        scaffoldCheck->setVisible(isNew);
        if (!isNew) {
            nameEdit->setPlaceholderText("Auto-detectado da pasta");
        } else {
            nameEdit->setPlaceholderText("ex: meu-novo-servico");
        }
    });

    vbox->addLayout(form);

    auto *btnRow = new QHBoxLayout();
    btnRow->addStretch();

    auto *btnCancel = new QPushButton("Cancelar", &dlg);
    btnCancel->setObjectName("btnCancel");
    connect(btnCancel, &QPushButton::clicked, &dlg, &QDialog::reject);
    btnRow->addWidget(btnCancel);

    auto *btnCreate = new QPushButton("Criar Projeto & Pasta", &dlg);
    btnCreate->setObjectName("btnCreate");
    btnCreate->setDefault(true);
    connect(btnCreate, &QPushButton::clicked, &dlg, &QDialog::accept);
    btnRow->addWidget(btnCreate);

    vbox->addLayout(btnRow);

    if (dlg.exec() == QDialog::Accepted) {
        bool isNew = (modeCombo->currentData().toString() == "NEW");
        QString finalDir;
        QString name = nameEdit->text().trimmed();

        if (isNew) {
            if (name.isEmpty()) {
                showToast("Informe o nome do projeto.", true);
                return;
            }
            QString parent = folderEdit->text().trimmed();
            if (parent.isEmpty() || !QDir(parent).exists()) {
                showToast("A pasta pai selecionada não existe no disco.", true);
                return;
            }
            finalDir = QDir::cleanPath(QDir(parent).filePath(name));
            QDir dirMaker;
            if (!dirMaker.mkpath(finalDir)) {
                showToast("Falha ao criar o diretório no disco.", true);
                return;
            }

            // Scaffolding if checked
            if (scaffoldCheck->isChecked()) {
                QString lang = langCombo->currentText();
                // README
                QFile readme(QDir(finalDir).filePath("README.md"));
                if (readme.open(QIODevice::WriteOnly | QIODevice::Text)) {
                    QTextStream out(&readme);
                    out << "# " << name << "\n\nProjeto gerenciado com DevHub Workspace.\nStack: " << lang << "\n";
                    readme.close();
                }

                // .gitignore
                QFile gitignore(QDir(finalDir).filePath(".gitignore"));
                if (gitignore.open(QIODevice::WriteOnly | QIODevice::Text)) {
                    QTextStream out(&gitignore);
                    out << "build/\nout/\n.vs/\nbin/\nobj/\ntarget/\nnode_modules/\n__pycache__/\n.env\n";
                    gitignore.close();
                }

                if (lang == "C++") {
                    QFile cmake(QDir(finalDir).filePath("CMakeLists.txt"));
                    if (cmake.open(QIODevice::WriteOnly | QIODevice::Text)) {
                        QTextStream out(&cmake);
                        out << "cmake_minimum_required(VERSION 3.20)\n"
                            << "project(" << name << " LANGUAGES CXX)\n\n"
                            << "set(CMAKE_CXX_STANDARD 20)\n"
                            << "set(CMAKE_CXX_STANDARD_REQUIRED ON)\n\n"
                            << "add_executable(" << name << " main.cpp)\n";
                        cmake.close();
                    }
                    QFile mainCpp(QDir(finalDir).filePath("main.cpp"));
                    if (mainCpp.open(QIODevice::WriteOnly | QIODevice::Text)) {
                        QTextStream out(&mainCpp);
                        out << "#include <iostream>\n\nint main() {\n    std::cout << \"Hello from " << name << "!\\n\";\n    return 0;\n}\n";
                        mainCpp.close();
                    }
                } else if (lang == "Rust") {
                    QFile cargo(QDir(finalDir).filePath("Cargo.toml"));
                    if (cargo.open(QIODevice::WriteOnly | QIODevice::Text)) {
                        QTextStream out(&cargo);
                        out << "[package]\nname = \"" << name.toLower() << "\"\nversion = \"0.1.0\"\nedition = \"2021\"\n\n[dependencies]\n";
                        cargo.close();
                    }
                    QDir(finalDir).mkdir("src");
                    QFile mainRs(QDir(finalDir).filePath("src/main.rs"));
                    if (mainRs.open(QIODevice::WriteOnly | QIODevice::Text)) {
                        QTextStream out(&mainRs);
                        out << "fn main() {\n    println!(\"Hello from " << name << "!\");\n}\n";
                        mainRs.close();
                    }
                } else if (lang == "Python") {
                    QFile pyproject(QDir(finalDir).filePath("pyproject.toml"));
                    if (pyproject.open(QIODevice::WriteOnly | QIODevice::Text)) {
                        QTextStream out(&pyproject);
                        out << "[project]\nname = \"" << name.toLower() << "\"\nversion = \"0.1.0\"\ndependencies = []\n";
                        pyproject.close();
                    }
                    QFile mainPy(QDir(finalDir).filePath("main.py"));
                    if (mainPy.open(QIODevice::WriteOnly | QIODevice::Text)) {
                        QTextStream out(&mainPy);
                        out << "def main():\n    print(\"Hello from " << name << "!\")\n\nif __name__ == \"__main__\":\n    main()\n";
                        mainPy.close();
                    }
                } else if (lang == "Go") {
                    QFile gomod(QDir(finalDir).filePath("go.mod"));
                    if (gomod.open(QIODevice::WriteOnly | QIODevice::Text)) {
                        QTextStream out(&gomod);
                        out << "module " << name.toLower() << "\n\ngo 1.22\n";
                        gomod.close();
                    }
                    QFile mainGo(QDir(finalDir).filePath("main.go"));
                    if (mainGo.open(QIODevice::WriteOnly | QIODevice::Text)) {
                        QTextStream out(&mainGo);
                        out << "package main\n\nimport \"fmt\"\n\nfunc main() {\n    fmt.Println(\"Hello from " << name << "!\")\n}\n";
                        mainGo.close();
                    }
                } else if (lang == "TypeScript") {
                    QFile pkg(QDir(finalDir).filePath("package.json"));
                    if (pkg.open(QIODevice::WriteOnly | QIODevice::Text)) {
                        QTextStream out(&pkg);
                        out << "{\n  \"name\": \"" << name.toLower() << "\",\n  \"version\": \"1.0.0\",\n  \"scripts\": {\n    \"start\": \"ts-node src/index.ts\"\n  }\n}\n";
                        pkg.close();
                    }
                    QDir(finalDir).mkdir("src");
                    QFile indexTs(QDir(finalDir).filePath("src/index.ts"));
                    if (indexTs.open(QIODevice::WriteOnly | QIODevice::Text)) {
                        QTextStream out(&indexTs);
                        out << "console.log(\"Hello from " << name << "!\");\n";
                        indexTs.close();
                    }
                }
            }
        } else {
            // Existing folder mode
            finalDir = folderEdit->text().trimmed();
            if (finalDir.isEmpty() || !QDir(finalDir).exists()) {
                showToast("A pasta selecionada não existe.", true);
                return;
            }
            if (name.isEmpty()) {
                name = QFileInfo(finalDir).fileName();
            }
        }

        // Check duplicate
        for (const auto& existing : m_projects) {
            if (QDir::toNativeSeparators(existing.path).toLower() == QDir::toNativeSeparators(finalDir).toLower()) {
                showToast("Este diretório já está cadastrado no DevHub.", true);
                return;
            }
        }

        domain::Project p;
        p.id = QString("proj-%1").arg(QDateTime::currentMSecsSinceEpoch());
        p.name = name;
        p.path = QDir::toNativeSeparators(finalDir);
        p.language = langCombo->currentText();
        p.cxxStandard = stdEdit->text().trimmed();
        p.buildSystem = buildEdit->text().trimmed();
        p.category = catCombo->currentText();
        p.preferredIde = ideCombo->currentText();
        p.preferredAiTool = aiCombo->currentText();
        p.lastAccessed = "Agora mesmo";

        m_repo.save(p);
        refreshTable();
        onOpenSidePeek(p.id);
        showToast(QString("✓ Projeto \"%1\" cadastrado com sucesso!").arg(p.name));
    }
}

void MainWindow::onAddProjectClicked() {
    onCreateProjectDialog();
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
        showToast(QString("✓ %1 novos projetos detectados e cadastrados!").arg(addedCount));
    } else {
        showToast("Nenhum novo repositório encontrado na pasta.", true);
    }
}

void MainWindow::onOpenSettings() {
    QDialog dlg(this);
    dlg.setWindowTitle("Preferências de Ferramentas");
    dlg.setFixedWidth(400);
    dlg.setStyleSheet(getDialogStylesheet(m_isDarkMode));

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
    connect(btnOk, &QPushButton::clicked, [this, &dlg]() {
        dlg.accept();
        showToast("✓ Preferências salvas com sucesso!");
    });
    form->addRow(btnOk);

    dlg.exec();
}

// CamaraUX Empty State Pattern (10865 & 10703)
QWidget* MainWindow::createEmptyStateWidget() {
    auto *widget = new QWidget();
    widget->setObjectName("emptyStateContainer");
    auto *layout = new QVBoxLayout(widget);
    layout->setContentsMargins(40, 60, 40, 60);
    layout->setSpacing(14);
    layout->setAlignment(Qt::AlignCenter);

    auto *iconLabel = new QLabel("🔍", widget);
    iconLabel->setStyleSheet("font-size: 38px; margin-bottom: 4px; background: transparent;");
    iconLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(iconLabel);

    m_emptyTitleLabel = new QLabel("Nenhum projeto encontrado", widget);
    m_emptyTitleLabel->setObjectName("emptyTitleLabel");
    m_emptyTitleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(m_emptyTitleLabel);

    m_emptyDescLabel = new QLabel("Tente ajustar o termo de pesquisa ou selecionar outra categoria.", widget);
    m_emptyDescLabel->setObjectName("emptyDescLabel");
    m_emptyDescLabel->setAlignment(Qt::AlignCenter);
    m_emptyDescLabel->setWordWrap(true);
    layout->addWidget(m_emptyDescLabel);

    m_emptyActionBtn = new QPushButton("Limpar busca", widget);
    m_emptyActionBtn->setObjectName("btnPrimaryPill");
    m_emptyActionBtn->setFixedWidth(180);
    layout->addWidget(m_emptyActionBtn, 0, Qt::AlignCenter);

    widget->setVisible(false);
    return widget;
}

// CamaraUX Non-Blocking Toast Pattern (10687-toast-alerta-mensagem-inline-qual-usar)
void MainWindow::showToast(const QString &message, bool isError) {
    if (!m_toastLabel) {
        m_toastLabel = new QLabel(this);
        m_toastLabel->setObjectName("camarauxToast");
        m_toastTimer = new QTimer(this);
        m_toastTimer->setSingleShot(true);
        connect(m_toastTimer, &QTimer::timeout, [this]() {
            if (m_toastLabel) m_toastLabel->hide();
        });
    }

    m_toastLabel->setText(message);
    if (isError) {
        m_toastLabel->setStyleSheet(R"(
            QLabel#camarauxToast {
                background-color: #ef4444;
                color: #ffffff;
                padding: 10px 24px;
                border-radius: 999px;
                font-size: 13px;
                font-weight: 600;
            }
        )");
    } else if (m_isDarkMode) {
        m_toastLabel->setStyleSheet(R"(
            QLabel#camarauxToast {
                background-color: #000000;
                color: #ffffff;
                padding: 10px 24px;
                border-radius: 999px;
                font-size: 13px;
                font-weight: 500;
                border: 1px solid #404040;
            }
        )");
    } else {
        m_toastLabel->setStyleSheet(R"(
            QLabel#camarauxToast {
                background-color: #0f172a;
                color: #ffffff;
                padding: 10px 24px;
                border-radius: 999px;
                font-size: 13px;
                font-weight: 500;
                border: 1px solid #334155;
            }
        )");
    }

    m_toastLabel->adjustSize();
    int x = (width() - m_toastLabel->width()) / 2;
    int y = height() - m_toastLabel->height() - 36;
    m_toastLabel->move(x, y);
    m_toastLabel->show();
    m_toastLabel->raise();
    m_toastTimer->start(2800);
}

void MainWindow::onToggleDarkMode() {
    m_isDarkMode = !m_isDarkMode;
    QSettings settings("DevHub", "DevHub");
    settings.setValue("darkMode", m_isDarkMode);

    setupCamaraUxTheme(m_isDarkMode);
    if (m_btnThemeToggle) {
        m_btnThemeToggle->setText(m_isDarkMode ? "☀️ Claro" : "🌙 Escuro");
    }
    refreshTable();
    showToast(m_isDarkMode ? "🌙 Modo Escuro ativado" : "☀️ Modo Claro ativado");
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    QMainWindow::resizeEvent(event);
    if (m_toastLabel && m_toastLabel->isVisible()) {
        int x = (width() - m_toastLabel->width()) / 2;
        int y = height() - m_toastLabel->height() - 36;
        m_toastLabel->move(x, y);
    }
}

void MainWindow::onClearSearch() {
    m_searchQuery.clear();
    if (m_searchInput) m_searchInput->clear();
    m_activeCategory = "ALL";
    auto pills = findChildren<QPushButton*>("filterPill");
    for (auto *p : pills) {
        bool match = (p->text() == "Todos");
        p->setProperty("active", match ? "true" : "false");
        p->style()->unpolish(p);
        p->style()->polish(p);
    }
    refreshTable();
}

} // namespace devhub::ui
