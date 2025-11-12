#include "mainwindow.hpp"

#include <QWidgetAction>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QLabel>
#include <QDate>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {

    setupUI();
    connectionSignals();
    initParsers();

    Notification::cacher = parser_cb->getCacher();
    Notification::show(this);

    binanse->getPriceCoin("BNB");
}

MainWindow::~MainWindow() {
    delete parser_cb;
    delete reg_parser;
}

void MainWindow::setupUI() {
    setWindowTitle("app");
    resize(400, 300);

    mainWindow = new QWidget(this);
    setCentralWidget(mainWindow);

    createMenu();
    createUI();
}

void MainWindow::connectionSignals() {
    QObject::connect(btnStart, &QPushButton::clicked, this, &MainWindow::onClickedButtonStart);
    QObject::connect(btnUpdate, &QPushButton::clicked, this, &MainWindow::onClickedButtonUpdate);
    QObject::connect(actionTabelCurrencyRates, &QAction::triggered, this, &MainWindow::onCurrencyRatesActivated);
    QObject::connect(actionDynamicsGraph, &QAction::triggered, this, &MainWindow::onDynamicsGraphActivated);
}

void MainWindow::initParsers() {
    parser_cb = new ParserCB;
    reg_parser = new RegularParser;

    parser_cb->makeRequest(QDate::currentDate().addDays(-1).toString("dd/MM/yyyy"), ParserCB::ParsingFlags::CACHEDATA);
    ListCurrencyRates = parser_cb->makeRequest("", ParserCB::ParsingFlags::CACHEDATA);

    QString msg = "Page parsing : " + parser_cb->getLastUrlRequest();
    outputResult->append(msg);
    msg = "Information from : " + parser_cb->getLastDate();
    outputResult->append(msg);
    msg = "Received " + QString::number(ListCurrencyRates.size()) + " currencies.";
    outputResult->append(msg);
    outputResult->append("");
}

void MainWindow::createMenu() {
    QMenuBar* menuBar = this->menuBar();

    QMenu* menuTabel = menuBar->addMenu("Windows");
    QMenu* menuDates = menuBar->addMenu("Date");

    QWidgetAction* actionEditDate = new QWidgetAction(this);
    editDate = new QLineEdit(this);
    editDate->setMaxLength(10);
    editDate->setText(QDate::currentDate().toString("dd/MM/yyyy"));
    actionEditDate->setDefaultWidget(editDate);
    menuDates->addAction(actionEditDate);

    actionTabelCurrencyRates = new QAction("Currency rates", this);
    menuTabel->addAction(actionTabelCurrencyRates);

    actionDynamicsGraph = new QAction("Dynamics graph", this);
    menuTabel->addAction(actionDynamicsGraph);

}

void MainWindow::createUI() {
    editValute = new QLineEdit(this);
    btnStart = new QPushButton("Convert", this);
    btnUpdate = new QPushButton("Update", this);
    outputResult = new QTextEdit(this);
    outputResult->setReadOnly(true);

    mainLayout = new QGridLayout(mainWindow);
    const int Row = 10, Col = 10;
    for (int row = 0; row < Row; ++row) {
        mainLayout->setRowStretch(row, 1);
        mainLayout->setRowMinimumHeight(row, 5);
    }
    for (int col = 0; col < Col; ++col) {
        mainLayout->setColumnStretch(col, 1);
        mainLayout->setColumnMinimumWidth(col, 5);
    }
    for (int row = 0; row < Row; ++row)
        for (int col = 0; col < Col; ++col)
            mainLayout->addWidget(new QWidget(this), row, col);

    mainLayout->addWidget(editValute, 0, 0, 1, 10);
    mainLayout->addWidget(outputResult, 1, 0, 8, 10);
    mainLayout->addWidget(btnStart, 9, 0, 1, 1);
    mainLayout->addWidget(btnUpdate, 9, 9, 1, 1);
}