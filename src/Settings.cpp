#include "Settings.hpp"

Error Settings::lastError = { 0 ,"" };

QJsonObject Settings::root = {};

bool Settings::readAllConfig() {
    QFile file(":/settings/config.json");
    
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        lastError.code = -1;
        lastError.msg = "Cannot open config file: " + file.errorString();
        return false;
    }
    
    QByteArray jsonData = file.readAll();
    file.close();
    
    return parseJsonConfig(jsonData);
}

bool Settings::parseJsonConfig(const QByteArray& jsonData) {
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData, &parseError);
    
    if (parseError.error != QJsonParseError::NoError) {
        lastError.code = -1;
        lastError.msg = "JSON parse error: " + parseError.errorString();
        return false;
    }
    
    if (!doc.isObject()) {
        lastError.code = -1;
        lastError.msg = "Invalid JSON structure";
        return false;
    }
    
    root = doc.object();
    
    return true;
}