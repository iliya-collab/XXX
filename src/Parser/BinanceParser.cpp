#include "Parser/BinanceParser.hpp"

ssize_t BinanceParser::getPriceCoin(const char* coin) {
    QString req = "https://api.binance.com/api/v3/ticker/price?symbol=%1USDT";
    req = req.arg(coin);
    qDebug() << req;
    return 0;
}