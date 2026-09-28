#include <QApplication>
#include <QIcon>
#include <QFont>
#include <QFontDatabase>
#include "ui/MainWindow.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("DevHub C++");
    app.setOrganizationName("DevHub");
    app.setWindowIcon(QIcon(":/resources/app_icon.ico"));

    // Set clean sans-serif typography
    QFont font("Inter", 10);
    if (!font.exactMatch()) {
        font = QFont("Segoe UI", 10);
    }
    app.setFont(font);

    devhub::ui::MainWindow window;
    window.show();

    return app.exec();
}
