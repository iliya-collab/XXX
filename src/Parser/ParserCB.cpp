#include "Parser/ParserCB.hpp"
#include <QEventLoop>

const char* ParserCB::url_dailyXML = "https://www.cbr.ru/scripts/XML_daily.asp";
const char* ParserCB::url_dynamicXML = "https://www.cbr.ru/scripts/XML_dynamic.asp";


QList<CurrencyRateCB> ParserCB::makeRequest(const QString& date_req, int flag) {
    QList<CurrencyRateCB> ListCurrencyRates;

    QNetworkReply *reply;

    url_request = url_dailyXML;
    date = date_req;

    if (date == "")
        date = QDate::currentDate().toString("dd/MM/yyyy");
    url_request = QString(url_dailyXML) + "?date_req=" + date;
    reply = manager->get(QNetworkRequest( QUrl(url_request) ));

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() == QNetworkReply::NoError)
        parseCurrencies(ListCurrencyRates, reply->readAll());
    else
        qDebug() << "Error:" << reply->errorString();

    reply->deleteLater();

    if (flag == ParsingFlags::CACHEDATA) {

        int fd;
        if ((fd = cacher->create_cache_data(date.toStdString().c_str())) == -1)
            qDebug() << ERR << "Error creating a temporary file";
        else {
            auto& lst = cacher->get_cache_files();

            /*std::string data;
            std::stringstream ss;
            for (auto i : ListCurrencyRates) {
                ss << QString("%1 %2 %3 %4 %5")
                    .arg(i.ID)
                    .arg(i.CharCode)
                    .arg(i.Name)
                    .arg(i.Nominal)
                    .arg(i.Value).toStdString() << std::endl;
            }
            data = ss.str();*/

            size_t write_size = 0;
            for (auto& i : ListCurrencyRates)
                write_size += cacher->write_cache_data(lst[lst.size()-1].fd, serializeCurrencyRate(i).data(), sizeof(CurrencyRateCB));
            lst[lst.size()-1].sz = write_size;
        }
    }

    return ListCurrencyRates;
}

QList<CurrencyRateCB> ParserCB::makeRequest(const QString& date_req1, const QString& date_req2, const QString& id, int flag) {
    QList<CurrencyRateCB> ListCurrencyRates;

    QNetworkReply *reply;

    url_request = url_dynamicXML;
    date = date_req1+"-"+date_req2;

    url_request = url_dynamicXML + QString("?date_req1=%1&date_req2=%2&VAL_NM_RQ=%3").arg(date_req1).arg(date_req2).arg(id);
    reply = manager->get(QNetworkRequest( QUrl(url_request) ));

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() == QNetworkReply::NoError)
        parseCurrencies(ListCurrencyRates, reply->readAll());
    else
        qDebug() << "Error:" << reply->errorString();

    reply->deleteLater();

    return ListCurrencyRates;
}

void ParserCB::parseCurrencies(QList<CurrencyRateCB>& ListCurrencyRates, const QByteArray &data) {
    QXmlStreamReader xml(data);
    while (!xml.atEnd() && !xml.hasError()) {
        QXmlStreamReader::TokenType token = xml.readNext();
        if (token == QXmlStreamReader::StartElement && (xml.name() == "Valute" || xml.name() == "Record"))
            ListCurrencyRates.append(parseValute(xml));
    }
}

CurrencyRateCB ParserCB::parseValute(QXmlStreamReader& xml) {
    CurrencyRateCB rate;
    while (!(xml.tokenType() == QXmlStreamReader::EndElement && (xml.name() == "Valute" || xml.name() == "Record"))) {
        if (xml.tokenType() == QXmlStreamReader::StartElement) {
            QString elem = xml.name().toString();
            if (elem == "Valute")
                rate.ID =xml.attributes().value("ID").toString();
            else if (elem == "Record") {
                rate.Date = xml.attributes().value("Date").toString();
                rate.ID = xml.attributes().value("Id").toString();
            }
            else if (elem == "CharCode")
                rate.CharCode = xml.readElementText();
            else if (elem == "Nominal")
                rate.Nominal = xml.readElementText().toInt();
            else if (elem == "Name")
                rate.Name = xml.readElementText();
            else if (elem == "Value")
                rate.Value = xml.readElementText().replace(',', '.').toDouble();
        }
        xml.readNext();
    }
    return rate;
}

QByteArray ParserCB::serializeCurrencyRate(const CurrencyRateCB& rate)
{
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    
    QDataStream out(&buffer);
    out.setVersion(QDataStream::Qt_6_0);

    out << rate.Date;
    out << rate.ID;
    out << rate.CharCode;
    out << rate.Name;
    out << rate.Nominal;
    out << rate.Value;
    
    buffer.close();
    return byteArray;
};

bool ParserCB::deserializeCurrencyRate(const QByteArray& byteArray, CurrencyRateCB& rate)
{
    QBuffer buffer;
    buffer.setData(byteArray);
    buffer.open(QIODevice::ReadOnly);
    
    QDataStream in(&buffer);
    in.setVersion(QDataStream::Qt_6_0);
    
    in >> rate.Date;
    in >> rate.ID;
    in >> rate.CharCode;
    in >> rate.Name;
    in >> rate.Nominal;
    in >> rate.Value;
    
    buffer.close();
    
    return in.status() == QDataStream::Ok;
}