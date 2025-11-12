#include "Parser/RegularParser.hpp"

// $search USD,EUR

bool RegularParser::parseString(const QString& str, const QList<CurrencyRateCB>& ListCurrency) {
    ListTokens = str.split(' ');
    this->ListCurrency = ListCurrency;
    for (auto i : tabel) {
        if (ListTokens[0] == i.macro_name) {
            i.mfunc(this, ListTokens);
            return true;
        }
    }
    return false;
}

QString RegularParser::getResult() {
    return result;
}


void msearch(RegularParser* parser, QStringList args) {
    parser->result = "Result $search :\n";
    if (args.size() >= 2) {
        QStringList list_args = args[1].split(',');
        int cnt = 0;

        for (auto i : parser->ListCurrency) {
            for (auto j : list_args) {
                if (i.CharCode == j || i.ID == j) {
                    parser->result += QString("   ID : %1 Name : %2 CharCode : %3 Nominal : %4 Value : %5 RUB\n")
                        .arg(i.ID)
                        .arg(i.Name)
                        .arg(i.CharCode)
                        .arg(i.Nominal)
                        .arg(QString::number(i.Value, 'f', 4));
                    cnt += 1;
                    break;
                }
            }

            if (cnt == list_args.size())
                break;
        }
    }
}