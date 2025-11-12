#ifndef _REGULAR_PARSER_
#define _REGULAR_PARSER_

#include "ParserCB.hpp"

#include <QString>
#include <QList>
#include <vector>

class RegularParser;

typedef void(*_macro_func)(RegularParser* parser, QStringList args);

typedef struct {
    char macro_name[16];
    _macro_func mfunc;
} TabelMacroFuncs;

void msearch(RegularParser* parser, QStringList args);

class RegularParser {
public:
    RegularParser() {
        tabel.push_back({"$search", msearch});
    }

    bool parseString(const QString& str, const QList<CurrencyRateCB>& ListCurrency);
    QString getResult();
    
private:

    friend void msearch(RegularParser* parser, QStringList args);

    std::vector<TabelMacroFuncs> tabel;
    QList<CurrencyRateCB> ListCurrency;

    QStringList ListTokens;
    QString result;
};

#endif