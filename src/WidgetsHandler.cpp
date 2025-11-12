#include "mainwindow.hpp"

void MainWindow::onClickedButtonStart() {
    QString data = editValute->text();
    if (!reg_parser->parseString(data, ListCurrencyRates)) {

        QStringList parts = data.split(' ');
        if (data != "") {
            double val = parts[0].toDouble();
            QString prefIn = parts[1];
            QString prefOut = parts[2];  
            
            double res = 0;
            double CurrencyIn = 1, CurrencyOut = 1;
            int cnt = 0;
            
            for (auto i : ListCurrencyRates) {
                if (i.CharCode == prefIn) {
                    prefIn = i.CharCode;
                    CurrencyIn = i.Value/i.Nominal;
                    cnt += 1;
                }
                if (i.CharCode == prefOut) {
                    prefOut = i.CharCode;
                    CurrencyOut = i.Value/i.Nominal;
                    cnt += 1;
                }
                if (cnt == 2)
                    break;
            }

            res = val * CurrencyIn / CurrencyOut;
        
            QDateTime currentDateTime = QDateTime::currentDateTime();
            QString msg = QString("%1\n%2 %3 ⟶ %4 %5\n")
                .arg(currentDateTime.toString("yyyy-MM-dd hh:mm:ss"))
                .arg(QString::number(val, 'f', 4))
                .arg(prefIn)
                .arg(QString::number(res, 'f', 4))
                .arg(prefOut);
            outputResult->append(msg);
        }
    }
    else
        outputResult->append(reg_parser->getResult());
}

void MainWindow::onClickedButtonUpdate() {
    
    outputResult->clear();
    QString msg = "Page parsing : " + parser_cb->getLastUrlRequest();
    outputResult->append(msg);
    msg = "Information from : " + editDate->text();
    outputResult->append(msg);
    msg = "Received " + QString::number(ListCurrencyRates.size()) + " currencies.";
    outputResult->append(msg);
    outputResult->append("");

    parser_cb->makeRequest(editDate->text());
    
    if (DTable != nullptr) {
        delete DTable;
        DTable = new DTableCurrencyRates(5, ListCurrencyRates.size(), editDate->text(), ListCurrencyRates, this);
    }
}

void MainWindow::onCurrencyRatesActivated() {
    DTable = new DTableCurrencyRates(5, ListCurrencyRates.size(), editDate->text(), ListCurrencyRates, this);
}

void MainWindow::onDynamicsGraphActivated() {
    DGraph = new DDynamicsGraph(this);
}

/*void MainWindow::onEditingFinishedSearch() {
    auto listStr = lineSearch->text().split(",");
    bool is_found = false;

    for (auto i : listCurrency) {
        for (auto j : listStr)
            if (i.ID == j || i.Name == j || i.CharCode == j) {
                QString msg = QString("Found :\n   ID : %1\n   Name : %2\n   CharCode : %3\n   Nominal : %4\n   Value : %5 RUB\n")
                    .arg(i.ID)
                    .arg(i.Name)
                    .arg(i.CharCode)
                    .arg(i.Nominal)
                    .arg(QString::number(i.Value, 'f', 4));
                outputResult->append(msg);
                is_found = true;
                break;
            }
        if (!is_found)
            outputResult->append("There are no matches!\n");
    }
}*/