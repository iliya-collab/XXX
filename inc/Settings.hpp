#ifndef _SETTINGS_
#define _SETTINGS_

#include <QFile>

#include "Notification/NotificationConfig.hpp"

#ifndef OPERATION_STATUS
#define OPERATION_STATUS

#define OK  "\033[32m✔\033[0m"
#define ERR "\033[31m✗\033[0m"
#define WAR "\033[33m⚠\033[0m"

#endif

typedef struct {
    int code;
    QString msg;
} Error;

class Settings {

private:

    static Error lastError;
    static QJsonObject root;

    static bool parseJsonConfig(const QByteArray& jsonData);

public:

    static bool readAllConfig();

    static Error getLastError() {
        return lastError;
    }

    static QJsonObject getRootObject() {
        return root;
    }

};


#endif