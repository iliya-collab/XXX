#ifndef _BINANCE_
#define _BINANCE_

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QString>

class BinanceParser {
private:
    const char* apiKey;

public:

    ssize_t getPriceCoin(const char* coin);

};

#endif