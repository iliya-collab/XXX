#ifndef _MAIN_WINDOW_
#define _MAIN_WINDOW_

#include <QApplication>
#include <QMainWindow>

#include <QGridLayout>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QCheckBox>

#include "Parser/ParserCB.hpp"
#include "Parser/BinanceParser.hpp"
#include "Parser/RegularParser.hpp"
#include "CustomWindowDialogs/DialogDynamicsGraph.hpp"
#include "CustomWindowDialogs/DialogTableCurrencyRates.hpp"
#include "Notification/Notification.hpp"
#include "Settings.hpp"
#include "StyleManager.hpp"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();



private:

    QWidget* mainWindow = nullptr;
    QGridLayout* mainLayout = nullptr;

    QLineEdit* editValute = nullptr; // Виджет для ввода команд и конверта валют
    QLineEdit* editDate = nullptr; // Виджет для задания даты, от которой берется курс (расположен в меню Date)
    QPushButton* btnStart = nullptr;
    QPushButton* btnUpdate = nullptr;
    QTextEdit* outputResult = nullptr;

    DTableCurrencyRates* DTable = nullptr;
    QAction* actionTabelCurrencyRates = nullptr;
    DDynamicsGraph* DGraph = nullptr;
    QAction* actionDynamicsGraph = nullptr;

    ParserCB* parser_cb;
    BinanceParser* binanse;
    RegularParser* reg_parser;

    QList<CurrencyRateCB> ListCurrencyRates;

    void setupUI();
    void connectionSignals();
    void initParsers();

    void createMenu();
    void createUI();

private slots:

    void onClickedButtonStart();
    void onClickedButtonUpdate();
    void onCurrencyRatesActivated();
    void onDynamicsGraphActivated();

};

#endif
