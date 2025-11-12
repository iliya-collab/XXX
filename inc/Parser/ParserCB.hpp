#ifndef _PARSER_CB_
#define _PARSER_CB_

#include <QXmlStreamReader>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <QList>
#include <QByteArray>
#include <QDataStream>
#include <QBuffer>

#include "CacheData.hpp"

struct CurrencyRateCB {
    QString Date;
    QString ID;
    QString CharCode;
    QString Name;
    int Nominal;
    double Value;
};

class ParserCB : public QObject {
    Q_OBJECT
public:

    ParserCB() {
        manager = new QNetworkAccessManager(this);
        cacher = new CacheData;
    }

    ~ParserCB() {
        delete manager;
        delete cacher;
    }

    QList<CurrencyRateCB> makeRequest(const QString& date_req, int flag = 0);
    QList<CurrencyRateCB> makeRequest(const QString& date_req1, const QString& date_req2, const QString& id, int flag = 0);

    QString getLastUrlRequest() {
        return url_request;
    }

    QString getLastDate() {
        return date;
    }

    CacheData* getCacher() {
        return cacher;
    }

    enum ParsingFlags {
        CACHEDATA = 1
    };

    static const char* url_dailyXML;
    static const char* url_dynamicXML;

    // Функция для преобразования структуры в QByteArray
    static QByteArray serializeCurrencyRate(const CurrencyRateCB& rate);

    // Функция для чтения структуры из QByteArray
    static bool deserializeCurrencyRate(const QByteArray& byteArray, CurrencyRateCB& rate);

private:
    

    QString url_request = "";
    QString date = "";

    QNetworkAccessManager* manager;

    CacheData* cacher;

    void parseCurrencies(QList<CurrencyRateCB>& ListCurrencyRates, const QByteArray &data);
    CurrencyRateCB parseValute(QXmlStreamReader& xml);

};

#endif