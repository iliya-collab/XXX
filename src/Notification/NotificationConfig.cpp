#include "Notification/NotificationConfig.hpp"

NotificationConfig::_Config NotificationConfig::conf = {};

void NotificationConfig::loadConfig(const QJsonObject& root) {
    QJsonObject notification = root.value("Notification").toObject();

    conf.title = notification.value("Title").toString();
    conf.enable = notification.value("Enable").toBool();
    conf.duration = notification.value("Duration").toInteger();
}

NotificationConfig::_Config NotificationConfig::getConfig() {
    return conf;
}