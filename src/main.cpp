#include "mainwindow.hpp"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // --------------- Настройка программы ---------------
    if (!Settings::readAllConfig()) {
        qDebug() << ERR << Settings::getLastError().msg;
        return Settings::getLastError().code;
    }
    NotificationConfig::loadConfig(Settings::getRootObject());
    // -----------------------------------------------------
    
    MainWindow win;

    // --------------- Настройка стилей ---------------
    win.setStyleSheet(StyleManager::loadStyleSheet("main.qss"));
    Notification::loadStyleSheet(StyleManager::loadStyleSheet("notification.qss"));
    // -----------------------------------------------------

    win.show();

    return app.exec();
}
