#ifndef _NOTIFICATION_CONFIG_
#define _NOTIFICATION_CONFIG_

#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

class NotificationConfig {
private:

    struct _Config {
        bool enable;        // Вкл/выкл уведомления
        QString title;      // Заголовок уведомления 
        ssize_t duration;   // Время отображения уведомления, по истечению которого оно закрывается
    };

    static _Config conf;

public:

    static void loadConfig(const QJsonObject& root);
    static _Config getConfig();

};

#endif